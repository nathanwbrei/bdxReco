// $Id$
//
//    File: ExtVetoDigiHit_factory.cc
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetoDigiHit_factory.h"
#include <ExtVeto/ExtVetoPMTHit.h>

#include <TT/TranslationTable.h>


//------------------
// init
//------------------
jerror_t ExtVetoDigiHit_factory::init(void) {
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoDigiHit_factory::brun(JEventLoop *eventLoop, int32_t runnumber) {
	jout << "ExtVetoDigiHit_factory::brun new run number: " << runnumber << endl;
	m_tt = 0;
	eventLoop->GetSingle(m_tt);
	if (m_tt == 0) {
		jerr << " unable to get the TranslationTable from this run!" << endl;
		return OBJECT_NOT_AVAILABLE;
	}
	return NOERROR;

}

//------------------
// evnt
//------------------
jerror_t ExtVetoDigiHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber) {

	ExtVetoDigiHit *m_ExtVetoDigiHit = 0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector<const ExtVetoPMTHit*> m_ExtVetoPMTHits;
	vector<const ExtVetoPMTHit*>::const_iterator it;
	const ExtVetoPMTHit* m_ExtVetoPMTHit;

	//1b: retrieve ExtVetoPMTHit objects
	loop->Get(m_ExtVetoPMTHits);

	for (it = m_ExtVetoPMTHits.begin(); it != m_ExtVetoPMTHits.end(); it++) {
		m_ExtVetoPMTHit = *it;

		m_ExtVetoDigiHit = new ExtVetoDigiHit();
		m_ExtVetoDigiHit->m_channel = *(m_ExtVetoPMTHit->m_channel.ext_veto);
		m_ExtVetoDigiHit->Q = m_ExtVetoPMTHit->Q;
		m_ExtVetoDigiHit->T = m_ExtVetoPMTHit->T;
		m_ExtVetoDigiHit->pedMean = m_ExtVetoPMTHit->pedMean;
		_data.push_back(m_ExtVetoDigiHit);
	}

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVetoDigiHit_factory::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoDigiHit_factory::fini(void) {
	return NOERROR;
}

