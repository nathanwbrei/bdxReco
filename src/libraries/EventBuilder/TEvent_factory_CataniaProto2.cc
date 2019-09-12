// $Id$
//
//    File: TEvent_factory_CataniaProto2.cc
// Created: Thu Dec 22 12:40:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>

#include <EventBuilder/TEvent.h>
#include <EventBuilder/TEventHeader.h>

#include <Calorimeter/CalorimeterDigiHit.h>
#include <IntVeto/IntVetoDigiHit.h>
#include <ExtVeto/ExtVetoDigiHit.h>

#include <Calorimeter/CalorimeterCluster.h>
#include <Calorimeter/CalorimeterHit.h>
#include <IntVeto/IntVetoHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <Paddles/PaddlesHit.h>

#include <DAQ/fa250Mode1Hit.h>
#include <MC/MCType.h>
#include <Calorimeter/CalorimeterMCRealHit.h>

#include <DAQ/eventData.h>

#include <JANA/JApplication.h>
#include "TClonesArray.h"

#include "TEvent_factory_CataniaProto2.h"
using namespace std;

//------------------
// init
//------------------
jerror_t TEvent_factory_CataniaProto2::init(void) {

	m_tag = "";
	gPARMS->GetParameter("MC", m_isMC);
	if (m_isMC) {
		jout << "CataniaProto2 event build - MC mode" << endl;
		gPARMS->GetParameter("MC:RUN_NUMBER", m_MCRunNumber);
		m_tag = "MC";
	}
	if ((m_isMC) && (m_isMC != MCType::CATANIA_V2)) {
		jout << "Error! Can use this only with MC==2, i.e. CataniaProto2" << endl;
		return VALUE_OUT_OF_RANGE;
	}

	m_root_lock = japp->GetService<JGlobalRootLock>();
	m_root_lock->acquire_write_lock();
	m_CaloDigiHits = new TClonesArray("CalorimeterDigiHit");
	m_IntVetoDigiHits = new TClonesArray("IntVetoDigiHit");
	m_ExtVetoDigiHits = new TClonesArray("ExtVetoDigiHit");

	m_CaloHits = new TClonesArray("CalorimeterHit");
	m_CaloClusters = new TClonesArray("CalorimeterCluster");
	m_IntVetoHits = new TClonesArray("IntVetoHit");
	m_ExtVetoHits = new TClonesArray("ExtVetoHit");

	if (m_isMC) {
		m_CaloMCRealHits = new TClonesArray("CalorimeterMCRealHit");
	}

	m_root_lock->release_lock();

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t TEvent_factory_CataniaProto2::brun(JEventLoop *eventLoop, int32_t runnumber) {
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t TEvent_factory_CataniaProto2::evnt(JEventLoop *loop, uint64_t eventnumber) {

	vector<const fa250Mode1Hit*> fahits;
	uint32_t fineTime = 0;

	vector<const CalorimeterDigiHit*> chits_digi;
	vector<const IntVetoDigiHit*> ivhits_digi;
	vector<const ExtVetoDigiHit*> evhits_digi;

	vector<const CalorimeterCluster*> cclusters;
	vector<const CalorimeterHit*> chits;
	vector<const IntVetoHit*> ivhits;
	vector<const ExtVetoHit*> evhits;

	vector<const CalorimeterMCRealHit*> chits_MCReal;

	const eventData* tData;

	/*Create the TEvent*/
	TEvent* m_event = new TEvent;
	TEventHeader *m_eventHeader = m_event->getEventHeader();
	/*Set the Event header*/
	if (!m_isMC) {
		try {
			loop->GetSingle(tData);
		} catch (unsigned long e) {
			jout << "TEvent_factory_CataniaProto2::evnt no eventData bank this event" << endl;
			return OBJECT_NOT_AVAILABLE;
		}
		m_eventHeader->setEventType(CataniaProto2Event);
		m_eventHeader->setRunNumber(tData->runN);
		m_eventHeader->setEventNumber(tData->eventN);
		m_eventHeader->setEventTime(tData->time);
		m_eventHeader->setTriggerWords(tData->triggerWords);

		loop->Get(fahits); /*just to get the event fine time from slot #4 - reading calo*/
		for (int ii = 0; ii < fahits.size(); ii++) {
			if (fahits[ii]->m_channel.slot != 4) continue;
			if (fahits[ii]->m_channel.slot == 4) {
				fineTime = fahits[ii]->timestamp;
				break;
			}
		}
		m_eventHeader->setEventFineTime(fineTime);

	} else {
		m_eventHeader->setEventType(CataniaProto2MC);
		m_eventHeader->setEventNumber(eventnumber);
		m_eventHeader->setEventTime(0);
		m_eventHeader->setEventFineTime(0);
		//	m_eventHeader->setTriggerWords(); /*A.C. we don't have any trigger simulation*/
		m_eventHeader->setRunNumber(m_MCRunNumber);
	}

	/*Loop over JANA objects, clear collections and fill them*/

	/*Digi objects*/
	loop->Get(chits_digi, m_tag.c_str());
	m_CaloDigiHits->Clear("C");
	for (int ii = 0; ii < chits_digi.size(); ii++) {
		((CalorimeterDigiHit*) m_CaloDigiHits->ConstructedAt(ii))->operator=(*(chits_digi[ii]));
		m_event->AddAssociatedObject(chits_digi[ii]);
	}
	m_event->addCollection(m_CaloDigiHits);

	loop->Get(ivhits_digi, m_tag.c_str());
	m_IntVetoDigiHits->Clear("C");
	for (int ii = 0; ii < ivhits_digi.size(); ii++) {
		((IntVetoDigiHit*) m_IntVetoDigiHits->ConstructedAt(ii))->operator=(*(ivhits_digi[ii]));
		m_event->AddAssociatedObject(ivhits_digi[ii]);
	}
	m_event->addCollection(m_IntVetoDigiHits);

	loop->Get(evhits_digi, m_tag.c_str());
	m_ExtVetoDigiHits->Clear("C");
	for (int ii = 0; ii < evhits_digi.size(); ii++) {
		((ExtVetoDigiHit*) m_ExtVetoDigiHits->ConstructedAt(ii))->operator=(*(evhits_digi[ii]));
		m_event->AddAssociatedObject(evhits_digi[ii]);
	}
	m_event->addCollection(m_ExtVetoDigiHits);

	/*Calibrated and final objects*/
	loop->Get(cclusters);
	m_CaloClusters->Clear("C");
	for (int ii = 0; ii < cclusters.size(); ii++) {
		((CalorimeterCluster*) m_CaloClusters->ConstructedAt(ii))->operator=(*(cclusters[ii]));
		m_event->AddAssociatedObject(cclusters[ii]);
	}
	m_event->addCollection(m_CaloClusters);

	loop->Get(chits);
	m_CaloHits->Clear("C");
	for (int ii = 0; ii < chits.size(); ii++) {
		((CalorimeterHit*) m_CaloHits->ConstructedAt(ii))->operator=(*(chits[ii]));
		m_event->AddAssociatedObject(chits[ii]);
	}
	m_event->addCollection(m_CaloHits);

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

	if (m_isMC) {
		loop->Get(chits_MCReal);
		m_CaloMCRealHits->Clear("C");
		for (int ii = 0; ii < chits_MCReal.size(); ii++) {
			((CalorimeterMCRealHit*) m_CaloMCRealHits->ConstructedAt(ii))->operator=(*(chits_MCReal[ii]));
			m_event->AddAssociatedObject(chits_MCReal[ii]);
		}
		m_event->addCollection(m_CaloMCRealHits);
	}

	/*publish the event*/
	_data.push_back(m_event);

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t TEvent_factory_CataniaProto2::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t TEvent_factory_CataniaProto2::fini(void) {
	m_root_lock->acquire_write_lock();
//	if (m_CaloHits!=0) delete (m_CaloHits);
//	if (m_IntVetoHits!=0) delete (m_IntVetoHits);
//	if (m_ExtVetoHits!=0) delete (m_ExtVetoHits);
	m_root_lock->release_lock();
	return NOERROR;
}

