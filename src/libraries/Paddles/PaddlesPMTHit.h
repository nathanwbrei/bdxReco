// $Id$
//
//    File: ExtVetoDigiHit.h
// Created: Wed Jan 13 21:06:40 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _PaddlesPMTHit_
#define _PaddlesPMTHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <DAQ/fa250ConvertedHit.h>
#include <TT/TranslationTable.h>


class PaddlesPMTHit:public fa250ConvertedHit{
public:
	JOBJECT_PUBLIC(PaddlesPMTHit);

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {}

public: //yes, these are public to keep this simple.

		//A.C. do not touch these
		TranslationTable::ChannelInfo m_channel; //both crate-slot channel and detector-specific ID. Since this is not a sensor-based object, the readout field will be =0
		double Q,T;
		uint64_t fa250Hit_id; //jana id for the fadc corresponding hit

};

#endif //

