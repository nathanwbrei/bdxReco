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
		
		// Add data members here. For example:
		// int id;
		// double E;
		double E,T;
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			 AddString(items, "sector", "%i",m_channel.sector);
			 AddString(items, "layer", "%i",m_channel.layer);
			 AddString(items, "component", "%i",m_channel.component);
			 AddString(items, "E", "%f", E);
			 AddString(items, "T", "%f", T);
		}
		TranslationTable::EXT_VETO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be ==0

		ClassDef(ExtVetoHit,1);
};

#endif // _ExtVetoHit_

