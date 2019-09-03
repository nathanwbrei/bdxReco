// $Id$
//
//    File: ExtVetoHit.h
// Created: Wed Jan 13 21:06:44 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoHit_
#define _ExtVetoHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>
#include "TObject.h"

class ExtVetoHit : public JObject, public TObject {
	public:
		JOBJECT_PUBLIC(ExtVetoHit);
		
		double E,T;

		void Summarize(JObjectSummary& summary) const final {
			summary.add(m_channel.sector, "sector", "%i");
			summary.add(m_channel.layer, "layer", "%i");
			summary.add(m_channel.component, "component", "%i");
			summary.add(E, "E", "%f");
			summary.add(T, "T", "%f");
		}

		TranslationTable::EXT_VETO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be ==0

		ClassDef(ExtVetoHit,1);
};

#endif // _ExtVetoHit_

