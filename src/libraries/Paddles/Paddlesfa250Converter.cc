#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode7Hit.h>

#include <Paddles/Paddlesfa250Converter.h>
#include <math.h>
#include <vector>

PaddlesPMTHit* Paddlesfa250Converter::convertHit(const fa250Hit *hit, const TranslationTable::ChannelInfo &m_channel, int eventN) const {
	PaddlesPMTHit *m_PaddlesPMTHit = new PaddlesPMTHit;
	m_PaddlesPMTHit->m_channel = m_channel;

	if (strcmp(hit->className().c_str(), "fa250Mode1Hit") == 0) {
		this->convertMode1Hit(m_PaddlesPMTHit, (const fa250Mode1Hit*) hit, m_channel);
	} else if (strcmp(hit->className().c_str(), "fa250Mode7Hit") == 0) {
		this->convertMode7Hit(m_PaddlesPMTHit, (const fa250Mode7Hit*) hit);
	} else {
		jerr << "Paddlesfa250Converter::convertHit unsupported class name: " << hit->className() << jendl;
		return 0;
	}
	return m_PaddlesPMTHit;
}

jerror_t Paddlesfa250Converter::convertMode1Hit(PaddlesPMTHit* output, const fa250Mode1Hit *input, const TranslationTable::ChannelInfo &m_channel) const {

	double Thr;
	Thr = threshold->getCalibSingle(*output->m_channel.paddles);

//	vector<double> m_pedestal_init;
//	m_pedestal_init=pedestal_init->getCalib(output->m_channel.paddles);
//	static double Ped_prev_id0=m_pedestal_init.at(0);
//	static double Ped_prev_id1=m_pedestal_init.at(1);

	int Nsamples = 30;

	static double Ped_prev_id0 = (m_pedestals->getRawCalibData().at(144).at(3)) * LSB;
	static double Ped_prev_id1 = (m_pedestals->getRawCalibData().at(145).at(3)) * LSB;

	double Ped = 0;
	double Q = 0;					// nC
	double T = 0;					// nsec
	double max = 0;

	double Tinf, Tsup, Amp1, Amp2;

	int T_index = -1;
	int max_index = -1;
	int inf_index = -1;
	int sup_index = -1;
	int size = input->samples.size();

//	jout<<"****************"<<std::endl;
//	jout<<"Channel= "<<m_channel.paddles->id<<" Thr= "<<Thr<<" "<<" Ped_prev_id_0= "<<Ped_prev_id0<<" Ped_prev_id_1= "<<Ped_prev_id1<<std::endl;

	/////////	 Leading-edge Timing  //////////
	for (int ii = 0; ii < size; ii++) {

		if (input->samples.at(ii) * LSB > Thr && ii == 0) {
			T_index = ii;
			T = 0;
			break;
		}
		if (input->samples.at(ii) * LSB > Thr && ii > 0) {
			Tinf = (ii - 1) * 4;
			Tsup = ii * 4;
			Amp1 = input->samples.at(ii - 1) * LSB;
			Amp2 = input->samples.at(ii) * LSB;
			T_index = ii;
//				jout<<"Amp over threshold= "<<input->samples.at(ii)<<" T_index= "<<T_index<<" "<<std::endl;
//				jout<<"Amp over threshold 1= "<<Amp1<<" Amp over threshold 2= "<<Amp2<<std::endl;
//				jout<<"T_inf= "<<Tinf<<" T_sup= "<<Tsup<<std::endl;
			break;
		}
	}
	if (T_index < 0)	// no signal
			{
		output->Q = 0;
		output->T = 0;
		return NOERROR;
	}

//			if(T_index>0)T=(((Tsup-Tinf)/(Amp2-Amp1))*(Thr-Amp1))+Tinf;		// linear interpolation	Leading-edge
//			jout<<"T_index= "<<T_index<<"Leading Edge= "<<T<<std::endl;

	/********************************************************************************************************************/

	/////////	 Pedestal  //////////
//		jout<<"Ped= "<<Ped<<" Ped_prev_id_0= " <<Ped_prev_id0<<" Ped_prev_id_1= "<<Ped_prev_id1<<std::endl;
	if (T_index < Nsamples && m_channel.paddles->id == 0) {
		Ped = Ped_prev_id0;
	} else if (T_index < Nsamples && m_channel.paddles->id == 1) {
		Ped = Ped_prev_id1;
	} else {
		for (int ii = 0; ii < Nsamples; ii++)
			Ped += input->samples.at(ii) * LSB;
		Ped = Ped / 30;
		if (m_channel.paddles->id == 0)
			Ped_prev_id0 = Ped;
		if (m_channel.paddles->id == 1)
			Ped_prev_id1 = Ped;
	}

	//jout<<"Ped= "<<Ped<<" Ped_prev_id0= "<<Ped_prev_id0<<" Ped_prev_id1= "<<Ped_prev_id1<<std::endl;

	/********************************************************************************************************************/

	/////////	 Charge and Maximum  //////////
	for (int ii = 0; ii < size; ii++) {
		Q += (input->samples.at(ii) * LSB - Ped);
		if (input->samples.at(ii) * LSB > max) {
			max = input->samples.at(ii) * LSB;
			max_index = ii;
			inf_index = ii - 3;			// integrate from 12 nsec before ...
			sup_index = ii + 3;			// .... to 12 nsec after the maximum
			if (inf_index < 0)
				inf_index = 0;
			if (sup_index > size - 1)
				sup_index = size - 1;
		}
	}
	//	jout<<"Max= "<<max<<" max_index= "<<max_index<<" inf_index= "<<inf_index<<" sup_index= "<<sup_index<<std::endl;

	if (T > 0 && Q < 0) {					// to recover most of the (few) "strange" signals (small and very fast (2-3 bins))
	//jout<<"Q_before= "<<Q<<std::endl;
		Q = 0;		// reset Q for new calculation
		for (int ii = inf_index; ii < sup_index + 1; ii++)
			Q += (input->samples.at(ii) * LSB - Ped);
		//jout<<"Q_after= "<<Q<<std::endl;
	}

	Q = 2 * Q * (0.001 * 4) / 50;        // from Wb to nCoulomb , 4 [nsec], 50 [Ohm], Q [Volts], 2 splitter

	/********************************************************************************************************************/

	/////////	 Constant fraction timing //////////
	if (T_index > 0) {

		double f = 0.5;
		int delay = 2;	// binning. 1 bin = 4 nsec
		double *Att_inv_pulse = new double[size];
		double *Delay_pulse = new double[size];
		double *Sum = new double[size];

		for (int ii = 0; ii < size; ii++) {
			Att_inv_pulse[ii] = 0;
			Delay_pulse[ii] = 0;
			Sum[ii] = 0;
		}

		for (int ii = 0; ii < size; ii++) {
			Att_inv_pulse[ii] = -f * (input->samples.at(ii) * LSB - Ped);
			if (ii >= delay)
				Delay_pulse[ii] = (input->samples.at(ii - delay) * LSB - Ped);
			Sum[ii] = Att_inv_pulse[ii] + Delay_pulse[ii];
//				cout<<ii<<" Signal= "<<input->samples.at(ii)<<" Att_inv= "<<Att_inv_pulse[ii]<<" Delay= "<<Delay_pulse[ii]<<" Sum= "<<Sum[ii]<<endl;
		}

//			jout<<max_index<<" "<<delay<<" "<<max_index+delay<<endl;
		for (int ii = (max_index + delay); ii > 0; ii--) {
			if (Sum[ii] < 0) {
				Tinf = (ii) * 4;
				Tsup = (ii + 1) * 4;
				Amp1 = Sum[ii];
				Amp2 = Sum[ii + 1];
				//					jout<<ii<<" "<<Amp1<<" "<<Amp2<<endl;
				break;
			}
		}

		T = ((((Tsup - Tinf) / (Amp2 - Amp1)) * (0 - Amp1)) + Tinf) - delay * 4;		// linear interpolation with Threshold = 0
//						jout<<"\e[1mCFD= \e[0m "<<T<<endl;
		delete Att_inv_pulse;
		delete Delay_pulse;
		delete Sum;
	}
	output->Q = Q;
	output->T = T;

	return NOERROR;
}

jerror_t Paddlesfa250Converter::convertMode7Hit(PaddlesPMTHit* output, const fa250Mode7Hit *input) const {

	output->Q = 0;
	output->T = 0;
	return NOERROR;
}
