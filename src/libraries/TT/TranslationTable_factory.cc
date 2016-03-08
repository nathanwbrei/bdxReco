// $Id$
//
//    File: TranslationTable_factory.cc
// Created: Tue Jan 12 09:29:14 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "TranslationTable_factory.h"
using namespace jana;

TranslationTable_factory::TranslationTable_factory():
						tt(0),isMC(0),VERBOSE(0){

	jout<<"TranslationTable_factory creator is called"<<endl;

	gPARMS->SetDefaultParameter("TT:VERBOSE", VERBOSE,
			"Verbosity level for Applying Translation Table."
			" 0=no messages, 10=all messages.");
	ROCID_MAP_FILENAME = "rocid.map";
	gPARMS->SetDefaultParameter("TT:ROCID_MAP_FILENAME", ROCID_MAP_FILENAME,
			"Optional rocid to rocid conversion map for use with files"
			" generated with the non-standard rocid's");

	NO_CCDB = false;
	gPARMS->SetDefaultParameter("TT:NO_CCDB", NO_CCDB,
			"Don't try getting translation table from CCDB and just look"
			" for file. Only useful if you want to force reading tt.xml."
			" This is automatically set if you specify a different"
			" filename via the TT:XML_FILENAME parameter.");
	XML_FILENAME = "tt.xml";
	gPARMS->SetDefaultParameter("TT:XML_FILENAME", XML_FILENAME,
				"Fallback filename of translation table XML file."
				" If set to non-default, CCDB will not be checked.");

	gPARMS->SetDefaultParameter("TT:SYSTEMS_TO_PARSE", SYSTEMS_TO_PARSE,
				"Comma separated list of systems to parse EVIO data for. "
				"Default is empty string which means to parse all. System "
				"names should be what is returned by DTranslationTable::DetectorName() .");
}




//------------------
// init
//------------------
jerror_t TranslationTable_factory::init(void)
{
	jout<<"TranslationTable_factory::init is called"<<endl;
	gPARMS->GetParameter("MC",isMC);
	gPARMS->GetParameter("TT:VERBOSE",VERBOSE);
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t TranslationTable_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{

	if (isMC==0){
		// Grab run-dependent translation table from CCDB
		tt = new TranslationTable(eventLoop);

		// Keep this translation table around and reuse it for
		// susequent events
		_data.push_back(tt);
		SetFactoryFlag(PERSISTANT);  /*This is very, very important: the Reset Method is not called at every event, so we do not clear data, and we
	can get the TranslationTable also in evnt method of other factories / event processors*/

		// If restricting parsing, make sure it is set for this source
		//	tt->SetSystemsToParse(loop->GetJEvent().GetJEventSource());
	}
	else{
		if (VERBOSE>3){
			jout<<"Working on MC, therefore no TT will be created"<<endl;
		}
	}
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t TranslationTable_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t TranslationTable_factory::erun(void)
{
	return NOERROR;

}

//------------------
// fini
//------------------
jerror_t TranslationTable_factory::fini(void)
{

	// If we have a translation table already the delete it
	if(tt){
		delete tt;
	}
	_data.clear();
	return NOERROR;
	return NOERROR;
}

