// $Id$
//
//    File: ExtVetoHit_factory.cc
// Created: Wed Jan 13 21:06:44 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetoDigiHit.h"
//#include "ExtVetoHit.h"

#include "ExtVetoHit_factory.h"
#include <TT/TranslationTable.h>
#include <JANA/JEvent.h>


ExtVetoHit_factory::ExtVetoHit_factory() :
		m_tt(0), m_ENE_gain(0) {
	isMC = 0;

	m_THR = 10;

	japp->SetDefaultParameter("EXTVETO:HIT_THR", m_THR, "Threshold in energy for a detector with single readout");

}

//------------------
// init
//------------------
void ExtVetoHit_factory::Init() {
	japp->GetParameter("MC", isMC);

	m_ENE_gain = new CalibrationHandler<TranslationTable::EXT_VETO_Index_t>("/ExtVeto/Ene");
	m_calibration_service = japp->GetService<BDXCalibrationService>();
	m_calibration_service->addCalibration(m_ENE_gain);
}

//------------------
// brun
//------------------
void ExtVetoHit_factory::ChangeRun(const std::shared_ptr<const JEvent>& event) {

	m_calibration_service->updateCalibration(m_ENE_gain, event->GetRunNumber(), event->GetEventNumber());
}

//------------------
// evnt
//------------------
void ExtVetoHit_factory::Process(const std::shared_ptr<const JEvent>& event) {

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector<const ExtVetoDigiHit*> m_ExtVetoDigiHits;
	vector<const ExtVetoDigiHit*>::const_iterator it;
	const ExtVetoDigiHit* m_ExtVetoDigiHit;

	double E, T, Qtot, Tmax, Emax;
	int nReadout, flagOk;
	double m_Ene;
	double Q, Qmax, m_EneCalib;

	ExtVetoHit *m_ExtVetoHit = 0;

	//1b: retrieve ExtVetoDigiHit objects

	/*This is very important!! Select - or not - the MC case*/
	if (isMC) {
		event->Get(m_ExtVetoDigiHits, "MC");
	} else {
		event->Get(m_ExtVetoDigiHits);
	}

	m_map.clear();
	for (it = m_ExtVetoDigiHits.begin(); it != m_ExtVetoDigiHits.end(); it++) {
		m_channel = ((*it)->m_channel);
		m_channel.readout = 0;
		m_map[m_channel].push_back(*it);
	}

	for (m_map_it = m_map.begin(); m_map_it != m_map.end(); m_map_it++) {
		//do here further elaborations!
		//Compute the charge as the sum of the charges
		//Compute the hit-time as time of the PMT-hit with largest charge
		m_ExtVetoDigiHits = m_map_it->second;
		m_ExtVetoHit = new ExtVetoHit();
		m_ExtVetoHit->m_channel = m_map_it->first;
		m_ExtVetoHit->E = 0;
		Qmax = -9999;
		Q = 0;
		for (int ihit = 0; ihit < m_ExtVetoDigiHits.size(); ihit++) {
			Q += m_ExtVetoDigiHits[ihit]->Q;
			if (m_ExtVetoDigiHits[ihit]->Q > Qmax) {
				Qmax = m_ExtVetoDigiHits[ihit]->Q;
				m_ExtVetoHit->T = m_ExtVetoDigiHits[ihit]->T;
			}
		}
		//	jout << "component "<< m_ExtVetoHit->m_channel.component << " " << Qmax<<endl;
		m_EneCalib = m_ENE_gain->getCalibSingle(m_ExtVetoHit->m_channel);
		Emax = Qmax * m_EneCalib;
	//	if (Emax < m_THR) continue;
		if (m_ExtVetoHit->T<0) continue;
		m_ExtVetoHit->E = Q * m_EneCalib;
		mData.push_back(m_ExtVetoHit); //publish it
	}
}

//------------------
// erun
//------------------
void ExtVetoHit_factory::EndRun() {

	m_calibration_service->clearCalibration(m_ENE_gain);
}

//------------------
// fini
//------------------
void ExtVetoHit_factory::Finish() {
}

