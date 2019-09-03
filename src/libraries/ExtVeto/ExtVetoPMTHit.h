// $Id$
//
//    File: ExtVetoDigiHit.h
// Created: Wed Jan 13 21:06:40 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoPMTHit_
#define _ExtVetoPMTHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <DAQ/fa250ConvertedHit.h>
#include <TT/TranslationTable.h>


class ExtVetoPMTHit : public fa250ConvertedHit{
public:
	JOBJECT_PUBLIC(ExtVetoPMTHit);


	void Summarize(JObjectSummary& summary) const final {
		summary.add(m_channel.ext_veto->sector, "sector", "%i");
		summary.add(m_channel.ext_veto->layer, "layer", "%i");
		summary.add(m_channel.ext_veto->component, "component", "%i");
		summary.add(m_channel.ext_veto->readout, "readout", "%i");
		summary.add(Q, "Q", "%f");
		summary.add(T, "T", "%f");
		summary.add(pedMean, "pedMean", "%f");
	}

public: //yes, these are public to keep this simple.

	//A.C. do not touch these
	TranslationTable::ChannelInfo m_channel; //both crate-slot channel and detector-specific ID. Since this is a sensor-based object, the readout field will be !=0

	double Q,T;
	double pedMean;
};

#endif //

