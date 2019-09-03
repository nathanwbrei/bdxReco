// $Id$
//
//    File: ExtVetoHit.h
// Created: Wed Jan 13 21:06:44 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _PaddlesHit_
#define _PaddlesHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include <TT/TranslationTable.h>

#include "TObject.h"


class PaddlesHit : public JObject, public TObject {
	public:
		JOBJECT_PUBLIC(PaddlesHit);
		
		// Add data members here. For example:
		double E,T;
		double x,y,z;
		
		// This method is used primarily for pretty printing

		void Summarize(JObjectSummary& summary) const final {};

		TranslationTable::PADDLES_Index_t m_channel;
		oid_t PaddlesDigiHit_id;

};

#endif // _PaddlesHit_

