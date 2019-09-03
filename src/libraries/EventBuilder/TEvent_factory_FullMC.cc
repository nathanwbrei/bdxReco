// $Id$
//
//    File: TEvent_factory_FullMC.cc
// Created: Thu Dec 22 12:40:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifdef MC_SUPPORT_ENABLE

#include <iostream>
#include <iomanip>

#include <EventBuilder/TEvent.h>
#include <EventBuilder/TEventHeader.h>

#include <Calorimeter/CalorimeterDigiHit.h>
#include <IntVeto/IntVetoDigiHit.h>
#include <ExtVeto/ExtVetoDigiHit.h>

#include <Calorimeter/CalorimeterHit.h>
#include <Calorimeter/CalorimeterCluster.h>
#include <IntVeto/IntVetoHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <Paddles/PaddlesHit.h>


#include <Calorimeter/CalorimeterMCRealHit.h>
#include <MC/GenParticle.h>


#include <DAQ/eventData.h>


#include <JANA/JApplication.h>
#include "TClonesArray.h"

#include "TEvent_factory_FullMC.h"
using namespace std;

//------------------
// init
//------------------
jerror_t TEvent_factory_FullMC::init(void) {

	m_tag = "";
	gPARMS->GetParameter("MC", m_isMC);
	if (m_isMC) {
		jout << "FullMC event build - MC mode" << endl;
		gPARMS->GetParameter("MC:RUN_NUMBER", m_MCRunNumber);
		m_tag = "MC";
	} else {
		jerr << "FullMC must be run in MC mode!" << endl;
		return VALUE_OUT_OF_RANGE;
	}
	if ((m_isMC) && (m_isMC != 10) && (m_isMC != 20)) {
		jout << "Error! Can use this only with MC=10 or 20, i.e. Full detector MC" << endl;
		return VALUE_OUT_OF_RANGE;
	}

	japp->RootWriteLock();
	/*	m_CaloDigiHits = new TClonesArray("CalorimeterDigiHit");
	 m_IntVetoDigiHits = new TClonesArray("IntVetoDigiHit");
	 m_ExtVetoDigiHits = new TClonesArray("ExtVetoDigiHit");
	 */
	m_CaloHits = new TClonesArray("CalorimeterHit");
	m_CaloClusters = new TClonesArray("CalorimeterCluster");
	m_IntVetoHits = new TClonesArray("IntVetoHit");
	m_ExtVetoHits = new TClonesArray("ExtVetoHit");

	m_GenParticles = new TClonesArray("GenParticle");
	/*
	 if (m_isMC){
	 m_CaloMCRealHits = new TClonesArray("CalorimeterMCRealHit");
	 }
	 */

	japp->RootUnLock();

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t TEvent_factory_FullMC::brun(JEventLoop *eventLoop, int32_t runnumber) {
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t TEvent_factory_FullMC::evnt(JEventLoop *loop, uint64_t eventnumber) {

	/*
	 vector<const CalorimeterDigiHit*> chits_digi;
	 vector<const IntVetoDigiHit*> ivhits_digi;
	 vector<const ExtVetoDigiHit*> evhits_digi;
	 vector<const CalorimeterMCRealHit*> chits_MCReal;
	 */
	vector<const CalorimeterHit*> chits;
	vector<const CalorimeterCluster*> cclusters;
	vector<const IntVetoHit*> ivhits;
	vector<const ExtVetoHit*> evhits;

	vector<const GenParticle*> genParticles;

	const eventData* tData;

	/*Create the TEvent*/
	TEvent* m_event = new TEvent;
	TEventHeader *m_eventHeader = m_event->getEventHeader();
	/*Set the Event header*/
	if (!m_isMC) {
		/*nothing to do now*/
	} else {
		m_eventHeader->setEventType(FullMC);
		m_eventHeader->setEventNumber(eventnumber);
		m_eventHeader->setEventTime(0);
		//	m_eventHeader->setTriggerWords(); /*A.C. we don't have any trigger simulation*/
		m_eventHeader->setRunNumber(m_MCRunNumber);
	}

	/*Loop over JANA objects, clear collections and fill them*/

	/*Digi objects - not enabled now*/
	/*
	 loop->Get(chits_digi,m_tag.c_str());
	 m_CaloDigiHits->Clear("C");
	 for (int ii = 0; ii < chits_digi.size(); ii++) {
	 ((CalorimeterDigiHit*) m_CaloDigiHits->ConstructedAt(ii))->operator=(*(chits_digi[ii]));
	 m_event->AddAssociatedObject(chits_digi[ii]);
	 }
	 m_event->addCollection(m_CaloDigiHits);

	 loop->Get(ivhits_digi,m_tag.c_str());
	 m_IntVetoDigiHits->Clear("C");
	 for (int ii = 0; ii < ivhits_digi.size(); ii++) {
	 ((IntVetoDigiHit*) m_IntVetoDigiHits->ConstructedAt(ii))->operator=(*(ivhits_digi[ii]));
	 m_event->AddAssociatedObject(ivhits_digi[ii]);
	 }
	 m_event->addCollection(m_IntVetoDigiHits);

	 loop->Get(evhits_digi,m_tag.c_str());
	 m_ExtVetoDigiHits->Clear("C");
	 for (int ii = 0; ii < evhits_digi.size(); ii++) {
	 ((ExtVetoDigiHit*) m_ExtVetoDigiHits->ConstructedAt(ii))->operator=(*(evhits_digi[ii]));
	 m_event->AddAssociatedObject(evhits_digi[ii]);
	 }
	 m_event->addCollection(m_ExtVetoDigiHits);
	 */

	/*Generated particles*/
	loop->Get(genParticles);
	m_GenParticles->Clear("C");
	for (int ii = 0; ii < genParticles.size(); ii++) {
		((GenParticle*) m_GenParticles->ConstructedAt(ii))->operator=(*(genParticles[ii]));
		m_event->AddAssociatedObject(genParticles[ii]);
	}
	m_event->addCollection(m_GenParticles);

	/*Calibrated and final objects*/
	loop->Get(chits);
	m_CaloHits->Clear("C");
	for (int ii = 0; ii < chits.size(); ii++) {
		((CalorimeterHit*) m_CaloHits->ConstructedAt(ii))->operator=(*(chits[ii]));
		m_event->AddAssociatedObject(chits[ii]);
	}
	m_event->addCollection(m_CaloHits);

	loop->Get(cclusters);
	m_CaloClusters->Clear("C");
	for (int ii = 0; ii < cclusters.size(); ii++) {
		((CalorimeterCluster*) m_CaloClusters->ConstructedAt(ii))->operator=(*(cclusters[ii]));
		m_event->AddAssociatedObject(cclusters[ii]);
	}
	m_event->addCollection(m_CaloClusters);

	loop->Get(ivhits);
	m_IntVetoHits->Clear("C");
	for (int ii = 0; ii < ivhits.size(); ii++) {
		((IntVetoHit*) m_IntVetoHits->ConstructedAt(ii))->operator=(*(ivhits[ii]));
		m_event->AddAssociatedObject(ivhits[ii]);
	}
	m_event->addCollection(m_IntVetoHits);

	loop->Get(evhits);
	m_ExtVetoHits->Clear("C");
	for (int ii = 0; ii < evhits.size(); ii++) {
		((ExtVetoHit*) m_ExtVetoHits->ConstructedAt(ii))->operator=(*(evhits[ii]));
		m_event->AddAssociatedObject(evhits[ii]);
	}
	m_event->addCollection(m_ExtVetoHits);
	/*
	 if (m_isMC){
	 loop->Get(chits_MCReal);
	 m_CaloMCRealHits->Clear("C");
	 for (int ii = 0; ii < chits_MCReal.size(); ii++) {
	 ((CalorimeterMCRealHit*) m_CaloMCRealHits->ConstructedAt(ii))->operator=(*(chits_MCReal[ii]));
	 m_event->AddAssociatedObject(chits_MCReal[ii]);
	 }
	 m_event->addCollection(m_CaloMCRealHits);
	 }*/

	/*publish the event*/
	_data.push_back(m_event);

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t TEvent_factory_FullMC::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t TEvent_factory_FullMC::fini(void) {
	japp->RootWriteLock();
//	if (m_CaloHits!=0) delete (m_CaloHits);
//	if (m_IntVetoHits!=0) delete (m_IntVetoHits);
//	if (m_ExtVetoHits!=0) delete (m_ExtVetoHits);
	japp->RootUnLock();
	return NOERROR;
}

#endif
