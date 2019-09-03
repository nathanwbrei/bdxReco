// $Id$
//
//    File: VetoIntDigiHit.h
// Created: Tue Jan 12 11:52:41 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _VetoIntSiPMHit_
#define _VetoIntSiPMHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <DAQ/fa250ConvertedHit.h>
#include <TT/TranslationTable.h>

#ifdef __ROOTCINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ class IntVetoSiPMHit+;
#endif

class IntVetoSiPMHit:public fa250ConvertedHit{
public:
	JOBJECT_PUBLIC(IntVetoSiPMHit);

	void Summarize(JObjectSummary& summary) const final {
		summary.add(m_channel.int_veto->sector, "sector", "%4d");
		summary.add(m_channel.int_veto->layer, "layer", "%4d");
		summary.add(m_channel.int_veto->component, "component", "%4d");
		summary.add(m_channel.int_veto->readout, "readout", "%4d");
		summary.add(Qraw, "Qraw", "%4f");
		summary.add(Qphe, "Qphe", "%4f");
		summary.add(T, "T", "%4f");
		summary.add(Araw, "Araw", "%4f");
		summary.add(Aphe, "Aphe", "%4f");
		summary.add(pedMean, "pedMean", "%4f");
		summary.add(pedRMS, "pedRMS", "%4f");
		summary.add(1*RMSflag, "rmsFlag", "%i");
		summary.add(m_type, "type", "%i");
	}

	typedef enum {
		noise=0,
		real_signal=1, //not much more to distinguish!
		num_hit_type
	} hit_type;



	//A.C. do not touch these
	//both crate-slot channel and detector-specific ID. Since this is a sensor-based object, the readout field will be !=0
	TranslationTable::ChannelInfo m_channel;

	double Qraw,Qphe;
	double T;
	double Araw;
	double Aphe;
	double average;
	double pedMean,pedRMS;
	int nSingles;
	bool RMSflag;
	hit_type m_type;
};

#endif // _VetoIntDigiHit_

