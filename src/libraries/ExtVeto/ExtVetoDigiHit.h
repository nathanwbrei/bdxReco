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

class ExtVetoDigiHit : public JObject, public TObject {
public:

	JOBJECT_PUBLIC(ExtVetoDigiHit);

	// Add data members here. For example:
	// int id;


	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string,string> > &items)const{
		/* AddString(items, "id", "%4d", id);*/
		AddString(items, "sector", "%i", m_channel.sector);
		AddString(items, "layer", "%i", m_channel.layer);
		AddString(items, "component", "%i", m_channel.component);
		AddString(items, "readout", "%i", m_channel.readout);
		AddString(items, "Q", "%f", Q);
		AddString(items, "T", "%f", T);
		AddString(items, "pedMean", "%f", pedMean);

	}
	//A.C. do not touch these
	TranslationTable::EXT_VETO_Index_t m_channel; //both crate-slot channel and detector-specific ID. Since this is a detector-based object, the readout field will be ==0
	double Q,T;
	double pedMean;

	ClassDef(ExtVetoDigiHit,1);

};

#endif // _ExtVetoDigiHit_

