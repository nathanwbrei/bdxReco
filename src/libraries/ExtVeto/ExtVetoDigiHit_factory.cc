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
void ExtVetoDigiHit_factory::Init() {}

//------------------
// brun
//------------------
void ExtVetoDigiHit_factory::ChangeRun(const std::shared_ptr<const JEvent>& event) {
	jout << "ExtVetoDigiHit_factory::brun new run number: " << runnumber << endl;
	m_tt = 0;
	eventLoop->GetSingle(m_tt);
	if (m_tt == 0) {
		jerr << " unable to get the TranslationTable from this run!" << endl;
	}
}

//------------------
// evnt
//------------------
void ExtVetoDigiHit_factory::Process(const std::shared_ptr<const JEvent>& aEvent) {

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
}

