// $Id$
//
//    File: PAddlesDigiHit.h
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _PaddlesDigiHit_
#define _PaddlesDigiHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>

#include "TObject.h"

/*
 * A.C.
 * This object represents a not-yet calibrated it in the Paddles.
 * The goal of this object is:
 * - Be as similar as possible to what is obtained from GEMC, while we wait GEMC producing composite, fadc-like, banks.
 */

class PaddlesDigiHit : public JObject, public TObject {
	public:
		JOBJECT_PUBLIC(PaddlesDigiHit);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			// AddString(items, "id", "%4d", id);
			// AddString(items, "E", "%f", E);
		}
		TranslationTable::PADDLES_Index_t m_channel; //both crate-slot channel and detector-specific ID. Since this is a detector-based object, the readout field will be ==0
		double Q,T;
		oid_t PaddlesPMTHit_id;

};

#endif // _PaddlesDigiHit_

