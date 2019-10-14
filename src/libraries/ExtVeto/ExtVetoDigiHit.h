// $Id$
//
//    File: ExtVetoDigiHit.h
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoDigiHit_
#define _ExtVetoDigiHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>

#include "TObject.h"
/*
 * A.C.
 * This object represents a not-yet calibrated it in the external veto.
 * The goal of this object is:
 * - Be as similar as possible to what is obtained from GEMC, while we wait GEMC producing composite, fadc-like, banks.
 */

class ExtVetoDigiHit : public TObject, public JObject {
public:

	JOBJECT_PUBLIC(ExtVetoDigiHit);


	void Summarize(JObjectSummary& summary) const final {
		summary.add(m_channel.sector, "sector", "%i");
		summary.add(m_channel.layer, "layer", "%i");
		summary.add(m_channel.component, "component", "%i");
		summary.add(m_channel.readout, "readout", "%i");
		summary.add(Q, "Q", "%f");
		summary.add(T, "T", "%f");
		summary.add(pedMean, "pedMean", "%f");
	}

	//A.C. do not touch these
	TranslationTable::EXT_VETO_Index_t m_channel; //both crate-slot channel and detector-specific ID. Since this is a detector-based object, the readout field will be ==0
	double Q,T;
	double pedMean;

	ClassDef(ExtVetoDigiHit,1);
};

#endif // _ExtVetoDigiHit_

