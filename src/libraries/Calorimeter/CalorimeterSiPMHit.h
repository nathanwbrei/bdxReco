// $Id$
//
//    File: CalorimeterSiPMHit.h
// Created: Wed Jan 27 16:39:49 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterSiPMHit_
#define _CalorimeterSiPMHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <DAQ/fa250ConvertedHit.h>
#include <TT/TranslationTable.h>


#include <TObject.h>


class CalorimeterSiPMHit:public fa250ConvertedHit{


public:
	JOBJECT_PUBLIC(CalorimeterSiPMHit);

	typedef enum{
		noise=0,
		one_phe=1,
		good_one_phe=2,
		many_phe=3,
		real_signal=4,
		num_hit_type
	}hit_type;


	// Add data members here. For example:
	// int id;
	// double E;

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {
		summary.add(m_channel.calorimeter->sector, "sector", "%i");

		summary.add(m_channel.calorimeter->sector, "sector", "%i");
		summary.add(m_channel.calorimeter->x, "x", "%i");
		summary.add(m_channel.calorimeter->y, "y", "%i");
		summary.add(m_channel.calorimeter->readout, "readout", "%i");
		summary.add(Qraw, "Qraw", "%f");
		summary.add(Qphe, "Qphe", "%f");
		summary.add(A, "A", "%f");
		summary.add(T, "T", "%f");
		summary.add(pedMean, "pedMean", "%f");
		summary.add(pedRMS, "pedRMS", "%f");
		summary.add(1*RMSflag, "rms_flag", "%i");
		summary.add(1*type, "type", "%i");
		summary.add(1*m_nSingles, "n_singles", "%i");
		summary.add(1*m_nSignals, "n_signals", "%i");
	}

	//A.C. do not touch these
	//both crate-slot channel and detector-specific ID. Since this is a sensor-based object, the readout field will be !=0
	TranslationTable::ChannelInfo m_channel;//!


	/*These 3 variables are: hit charge (u.a.), hit time (in ns), hit amplitude (in mV)*/
	double Qraw,Qphe;
	double T,A;
	double average;
	double pedMean,pedRMS;
	bool RMSflag;
	hit_type type;

	uint64_t timestamp;

	int m_nSingles,m_nSignals;

	ClassDef(CalorimeterSiPMHit,1);


};
#endif // _CalorimeterSiPMHit_

