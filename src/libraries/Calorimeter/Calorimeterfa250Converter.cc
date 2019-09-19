#include "Calorimeterfa250Converter.h"

#include "TF1.h"
#include "TH1D.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TApplication.h"

#include "Math/WrappedTF1.h"
#include "Math/WrappedMultiTF1.h"
#include "Fit/BinData.h"
#include "Fit/UnBinData.h"
#include "HFitInterface.h"
#include "Fit/Fitter.h"
#include "Math/Minimizer.h"

#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <DAQ/fa250Mode7Hit.h>

#include <DAQ/fa250Mode1CalibPedSubHit.h>

double fSinglePhe2Pole(double *x, double *par) {
	double ret = 0;
	double t = x[0] - par[0];
	if (t > 0) {
		ret = par[3] + (par[1] / (2 * par[2] * par[2] * par[2])) * t * t * exp(-t / par[2]); // the overall integral is par[1]. Max val is (2*par[1]/par[2])*exp(-2)

	} else {
		ret = par[3];
	}
	return ret;
}

double fSinglePhe1Pole(double *x, double *par) {
	double ret = 0;
	double t = x[0] - par[0];
	if (t > 0) {
		ret = par[3] + (par[1] / (par[2] * par[2])) * t * exp(-t / par[2]); // the overall integral is par[1]. Max val is (par[1]/par[2])*exp(-1)
	} else {
		ret = par[3];
	}
	return ret;
}

CalorimeterSiPMHit* Calorimeterfa250Converter::convertHit(const fa250Hit *hit, const TranslationTable::ChannelInfo &m_channel, int eventN) const {
	CalorimeterSiPMHit *m_CalorimeterSiPMHit = new CalorimeterSiPMHit;
	m_CalorimeterSiPMHit->m_channel = m_channel;
	m_CalorimeterSiPMHit->timestamp = hit->timestamp;

	if (strcmp(hit->className().c_str(), "fa250Mode1CalibPedSubHit") == 0) {
		this->convertMode1Hit(m_CalorimeterSiPMHit, (const fa250Mode1CalibPedSubHit*) hit);
	} else if (strcmp(hit->className().c_str(), "fa250Mode7Hit") == 0) {
		this->convertMode7Hit(m_CalorimeterSiPMHit, (const fa250Mode7Hit*) hit);
	} else {
		jerr << "Calorimeterfa250Converter::convertHit unsupported class name: " << hit->className() << jendl;
		return 0;
	}
	return m_CalorimeterSiPMHit;
}

jerror_t Calorimeterfa250Converter::convertMode1Hit(CalorimeterSiPMHit* output, const fa250Mode1CalibPedSubHit *input) const {
	int size = input->samples.size();
	int N, n, idx;
	double min, max, xmin, xmax, prev_xmin, prev_xmax, rms, Tmax;

	int m_NPEDs, m_NSBs, m_NSAs, m_NSAMPLESs;

	m_NPEDs = (int) (m_NPED / input->m_dT);
	m_NSBs = (int) (m_NSB / input->m_dT);
	m_NSAs = (int) (m_NSA / input->m_dT);
	m_NSAMPLESs = (int) (m_NSAMPLES / input->m_dT);

	/*Get thr and TOT from DB*/
	/* THR in mV
	 * m_MIN_TOT in ns
	 * m_SINGLE_SIGNAL_TOT in ns
	 */
	m_calib.clear();
	m_calib = m_thrDB->getCalib(*(output->m_channel.calorimeter));

	m_THR = m_calib[0];
	m_MIN_TOT = m_calib[1];
	m_SIGNAL_TOT = m_calib[2];

	output->Qraw = 0;
	output->T = 0;
	output->A = 0;
	output->type = CalorimeterSiPMHit::noise;

	output->AddAssociatedObject(input);

	vector<double> m_waveform;
	m_waveform.clear();

	std::pair<int, int> m_thisCrossingTime;

	std::vector<std::pair<int, int> > m_crossingTimes;
	std::vector<int> m_crossingTimesDelta;

	std::vector<int> m_singleCrossingIndexes;
	std::vector<int> m_signalCrossingIndexes;

	output->m_nSingles = 0;
	output->m_nSignals = 0;

	output->average = 0;
	//1: compute the average
	for (int ii = 0; ii < size; ii++) {
		output->average += input->samples[ii];
	}
	output->average /= input->samples.size();
	//1a: compute the pedestal
	output->pedMean = 0;
	output->pedRMS = 0;
	for (int ii = 0; ii < m_NPEDs; ii++) {
		output->pedMean += input->samples[ii];
		output->pedRMS += input->samples[ii] * input->samples[ii];
	}
	output->pedRMS /= m_NPEDs;
	output->pedMean /= m_NPEDs;
	output->pedRMS = sqrt(output->pedRMS - output->pedMean * output->pedMean);

	if (output->pedRMS <= input->m_RMS) {
		output->RMSflag = true;
	} else
		output->RMSflag = false;

	//1b produced the waveform
	for (int ii = 0; ii < size; ii++) {
		m_waveform.push_back(input->samples[ii] - output->pedMean);
	}

	//2: find m_THR crossings
	m_thisCrossingTime.first = -1;
	m_thisCrossingTime.second = -1;
	if (m_waveform[0] > m_THR) m_thisCrossingTime.first = 0;

	for (int ii = 1; ii < size; ii++) {
		if ((m_waveform[ii] > m_THR) && (m_waveform[ii - 1] < m_THR)) m_thisCrossingTime.first = ii;
		else if ((m_waveform[ii] < m_THR) && (m_waveform[ii - 1] > m_THR) && (m_thisCrossingTime.first != -1)) {
			m_thisCrossingTime.second = ii;
			m_crossingTimes.push_back(m_thisCrossingTime);
			m_thisCrossingTime.first = -1;
			m_thisCrossingTime.second = -1;
		}
	}
	//It may happen that the last sample is still over m_THR!
	if (m_waveform[size - 1] > m_THR) {
		m_thisCrossingTime.second = size;
		m_crossingTimes.push_back(m_thisCrossingTime);
	}

	//3: deltas
	//3a: if no crossing times are found

	/*Compute ToT */
	for (int itime = 0; itime < m_crossingTimes.size(); itime++) {
		m_crossingTimesDelta.push_back(m_crossingTimes[itime].second - m_crossingTimes[itime].first);
	}

	/*Verify the ToT for each pulse*/
	for (int itime = 0; itime < m_crossingTimes.size(); itime++) {

		if (m_crossingTimesDelta[itime] < 0) {
			jerr << "Calorimeterfa20Converter::convertMode1Hit error, negative ToT?" << jendl;
		} else if (m_crossingTimesDelta[itime] > m_SIGNAL_TOT / input->m_dT) {
			m_signalCrossingIndexes.push_back(itime);
		} else if ((m_crossingTimesDelta[itime] > m_MIN_TOT / input->m_dT) || (m_crossingTimes[itime].second) == (size) || (m_crossingTimes[itime].first) == (0)) {
			m_singleCrossingIndexes.push_back(itime);
		}
	}

	output->m_nSignals = m_signalCrossingIndexes.size();
	output->m_nSingles = m_singleCrossingIndexes.size();

	if (((output->m_nSignals) == 0) && (output->m_nSingles) == 0) {
		output->type = CalorimeterSiPMHit::noise;
		output->T = 0;
		if ((m_NSBs + m_NSAs) >= m_waveform.size()) {
			output->Qraw = this->sumSamples(m_waveform.size(), &(m_waveform[0])); //to be uniform with the case below
		} else {
			output->Qraw = this->sumSamples(0, m_NSBs + m_NSAs, &(m_waveform[0])); //to be uniform with the case below
		}
		output->A = 0;
		return NOERROR;
	} else if (output->m_nSignals >= 1) {
		output->type = CalorimeterSiPMHit::real_signal;

		idx = m_signalCrossingIndexes[0];
		xmax = m_crossingTimes[idx].first;  //this is the time of the sample OVER m_THR
		xmin = xmax - m_NSBs;
		xmax = xmax - m_NSBs + m_NSAMPLESs;
		if (xmin < 0) xmin = 0;
		if (xmax >= size) xmax = size - 1;

		output->Qraw = this->sumSamples((int) xmin, (int) xmax, &(m_waveform[0]));
		output->A = this->getMaximum(m_waveform.size(), &(m_waveform[0]), Tmax);

		xmax = m_crossingTimes[idx].first;  //this is the time of the sample OVER m_THR
		xmin = m_crossingTimes[idx].first - 1;
		if (xmin == 0) output->T = 0;
		else {
			max = m_waveform[xmax];
			min = m_waveform[xmin];
		}

		output->T = (m_THR - min) * (xmax - xmin) / (max - min) + xmin;
		output->T *= input->m_dT; //in NS!!!

	} else if ((output->m_nSignals == 0) && (output->m_nSingles == 1)) {
		output->A = this->getMaximum(m_crossingTimes[0].first, m_crossingTimes[0].second, &(m_waveform[0]), Tmax);

		if ((Tmax <= m_NSBs) || (Tmax >= (size - 1 - m_NSAs))) {
			output->type = CalorimeterSiPMHit::one_phe;
			if ((m_NSBs + m_NSAs) >= m_waveform.size()) {
				output->Qraw = this->sumSamples(m_waveform.size(), &(m_waveform[0]));
			} else {
				output->Qraw = this->sumSamples(0, m_NSBs + m_NSAs, &(m_waveform[0]));
			}

			output->T = 0;
			return NOERROR;
		} else {
			/*this instruction is to do sipm studies*/
			output->Qraw = this->sumSamples(m_waveform.size(), &(m_waveform[0]));

			output->type = CalorimeterSiPMHit::good_one_phe;
			xmin = Tmax - m_NSBs;
			xmax = Tmax + m_NSAs;

			/*This should never happen*/
			if (xmin < 0) xmin = 0;
			if (xmax >= size) xmax = (size - 1);

			N = int((xmax - xmin)) + 1;

			output->average /= m_waveform.size();
			output->T = Tmax;
			/*now timing*/
			xmax = m_crossingTimes[0].first; //first sample above m_THR
			xmin = xmax - 1; //sample befor m_THR
			max = m_waveform[xmax];
			min = m_waveform[xmin];

			output->T = (m_THR - min) * (xmax - xmin) / (max - min) + xmin;
			output->T *= input->m_dT; //in NS!!!

		}
	} else {
		output->type = CalorimeterSiPMHit::many_phe;
		output->A = 0;
		output->Qraw = 0;
		prev_xmin = 0;
		for (int iphe = 0; iphe < output->m_nSingles; iphe++) {
			idx = m_singleCrossingIndexes[iphe];
			xmin = m_crossingTimes[idx].first;
			xmax = m_crossingTimes[idx].second;
			max = this->getMaximum((int) xmin, (int) xmax, &(m_waveform[0]), Tmax);
			if ((output->A) < max) output->A = max;

			xmin = Tmax - m_NSBs;
			xmax = Tmax + m_NSAs;

			if (xmin < prev_xmin) xmin = prev_xmin;
			if (xmax > size) xmax = (size - 1);

			output->Qraw += this->sumSamples((int) xmin, (int) xmax, &(m_waveform[0]));
		}
		idx = m_singleCrossingIndexes[0];
		xmax = m_crossingTimes.at(idx).first;  //this is the time of the sample OVER m_THR
		xmin = m_crossingTimes.at(idx).first - 1;
		if (xmin == 0) output->T = 0;
		else {
			max = m_waveform[xmax];
			min = m_waveform[xmin];
		}
		output->T = (m_THR - min) * (xmax - xmin) / (max - min) + xmin;
		output->T *= input->m_dT; //in NS!!!

	}

	/*This part of the code was added to handle BDXmini setup, where there are two kind of digitizers:
	 * V1725 @250 MHz
	 * V1730 @500 MHz
	 *
	 * There are calorimeter channels connected to both, and the sampling time is really a property of the DAQ, not of the detector.
	 * Therefore, I want Qraw to already reflect this.
	 *  If I multiply Qraw by the sampling time, this is corrected
	 * - signals sampled at larger frequency have a lower sampling time, but more samples
	 * - the product is constant
	 * However, this would modify the code leaving it non retro-compatible, i.e. all previous cal. constants for the crystal would have to be replaced.
	 * Hence, the following code that does not alter anything if input->m_dT = 4 (ns), the usual sampling time we had before
	 */

	output->Qraw = output->Qraw * (input->m_dT / 4.);

	return NOERROR;
}

jerror_t Calorimeterfa250Converter::convertMode7Hit(CalorimeterSiPMHit* output, const fa250Mode7Hit *input) const {
	output->AddAssociatedObject(input);

	return NOERROR;
}

