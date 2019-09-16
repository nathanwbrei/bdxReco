// $Id$
//
//    File: TranslationTable_factory.cc
// Created: Tue Jan 12 09:29:14 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "TranslationTable_factory.h"
#include <JANA/JEvent.h>

#include <iostream>
#include <iomanip>


TranslationTable_factory::TranslationTable_factory() :
		tt(nullptr), isMC(0), VERBOSE(0) {

	japp->SetDefaultParameter("TT:VERBOSE", VERBOSE, "Verbosity level for Applying Translation Table."
			" 0=no messages, 10=all messages.");
	ROCID_MAP_FILENAME = "rocid.map";
	japp->SetDefaultParameter("TT:ROCID_MAP_FILENAME", ROCID_MAP_FILENAME, "Optional rocid to rocid conversion map for use with files"
			" generated with the non-standard rocid's");

	NO_CCDB = false;
	japp->SetDefaultParameter("TT:NO_CCDB", NO_CCDB, "Don't try getting translation table from CCDB and just look"
			" for file. Only useful if you want to force reading tt.xml."
			" This is automatically set if you specify a different"
			" filename via the TT:XML_FILENAME parameter.");
	XML_FILENAME = "tt.xml";
	japp->SetDefaultParameter("TT:XML_FILENAME", XML_FILENAME, "Fallback filename of translation table XML file."
			" If set to non-default, CCDB will not be checked.");

	japp->SetDefaultParameter("TT:SYSTEMS_TO_PARSE", SYSTEMS_TO_PARSE, "Comma separated list of systems to parse EVIO data for. "
			"Default is empty string which means to parse all. System "
			"names should be what is returned by DTranslationTable::DetectorName() .");
	if (VERBOSE > 2) {
		jout << "TranslationTable_factory creator is called. Pointer is: " << this << jendl;
	}
}


void TranslationTable_factory::Init() {
	jout << "TranslationTable_factory::init is called" << jendl;
	japp->GetParameter("MC", isMC);
	japp->GetParameter("TT:VERBOSE", VERBOSE);
}


void TranslationTable_factory::ChangeRun(const std::shared_ptr<const JEvent>& event) {

	if (isMC == 0) {
		// Grab run-dependent translation table from CCDB
		if (VERBOSE > 3) {
			jout << "TranslationTable_factory::brun -> TT will be created" << jendl;
		}
		tt = new TranslationTable(event->GetJApplication(), event->GetRunNumber());

		// Keep this translation table around and reuse it for susequent events
		Insert(tt);
		SetFactoryFlag(PERSISTANT); /*This is very, very important: the Reset Method is not called at every event, so we do not clear data, and we
		 can get the TranslationTable also in evnt method of other factories / event processors*/

		// If restricting parsing, make sure it is set for this source
		//	tt->SetSystemsToParse(loop->GetJEvent().GetJEventSource());
	} else {
		if (VERBOSE > 3) {
			jout << "Working on MC, therefore no TT will be created" << jendl;
		}
	}
}


void TranslationTable_factory::Process(const std::shared_ptr<const JEvent>& aEvent) {}



