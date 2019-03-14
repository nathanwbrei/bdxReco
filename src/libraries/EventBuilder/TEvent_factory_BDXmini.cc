#include <iostream>
#include <iomanip>
using namespace std;

#include <EventBuilder/TEvent.h>
#include <EventBuilder/TEventHeader.h>

#include <Calorimeter/CalorimeterHit.h>
#include <Calorimeter/CalorimeterDigiHit.h>
#include <IntVeto/IntVetoHit.h>

#include <DAQ/eventData.h>
#include <MC/MCType.h>
#ifdef MC_SUPPORT_ENABLE
#include <Calorimeter/CalorimeterMCRealHit.h>
#include <MC/GenParticle.h>
#include <MC/UserMCData.h>
#endif

#include "TClonesArray.h"
#include "TEvent_factory_BDXmini.h"

#include <JANA/JApplication.h>
using namespace jana;

//------------------
// init
//------------------
jerror_t TEvent_factory_BDXmini::init(void) {

	m_tag = "";
	gPARMS->GetParameter("MC", m_isMC);
	if (m_isMC) {
		jout << "BDXmini event build - MC mode" << endl;
		gPARMS->GetParameter("MC:RUN_NUMBER", m_MCRunNumber);
		m_tag = "MC";
	}
	if ((m_isMC) && (m_isMC != MCType::BDXmini_V1)) {
		jout << "Error! Can use this only with MC==200, i.e. BDXmini" << endl;
		return VALUE_OUT_OF_RANGE;
	}

	japp->RootWriteLock();

	m_CaloHits = new TClonesArray("CalorimeterHit");
	m_IntVetoHits = new TClonesArray("IntVetoHit");
#ifdef MC_SUPPORT_ENABLE
	m_GenParticles = new TClonesArray("GenParticle");
	m_CaloMCRealHits = new TClonesArray("CalorimeterMCRealHit");
#endif
	japp->RootUnLock();

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t TEvent_factory_BDXmini::brun(jana::JEventLoop *eventLoop, int32_t runnumber) {
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t TEvent_factory_BDXmini::evnt(JEventLoop *loop, uint64_t eventnumber) {

	/*Create the TEvent*/
	TEvent* m_event = new TEvent();
	TEventHeader *m_eventHeader = m_event->getEventHeader();

	const eventData* tData;

	vector<const CalorimeterHit*> chits;
	vector<const CalorimeterDigiHit*> cdhits;
	vector<const IntVetoHit*> ivhits;
#ifdef MC_SUPPORT_ENABLE
	vector<const CalorimeterMCRealHit*> chits_MCReal;
	vector<const GenParticle*> genParticles;
	vector<const UserMCData*> userMCdata;
#endif

	if (!m_isMC) {
		try {
			loop->GetSingle(tData);
		} catch (unsigned long e) {
			jout << "TEvent_factory_BDXmini::evnt no eventData bank this event" << endl;
			return OBJECT_NOT_AVAILABLE;
		}

		m_eventHeader->setEventType(BDXminiEvent);
		m_eventHeader->setRunNumber(tData->runN);
		m_eventHeader->setEventNumber(tData->eventN);
		m_eventHeader->setEventTime(tData->time);
		m_eventHeader->setTriggerWords(tData->triggerWords);
		m_eventHeader->setEventFineTime(0); //TODO
		m_eventHeader->setWeight(1);
	} else {
		m_eventHeader->setEventType(BDXminiEvent);
		m_eventHeader->setEventNumber(eventnumber);
		m_eventHeader->setEventTime(0);
		m_eventHeader->setEventFineTime(0);
		//	m_eventHeader->setTriggerWords(); /*A.C. we don't have any trigger simulation*/
		m_eventHeader->setRunNumber(m_MCRunNumber);
	}

	/*Loop over JANA objects, clear collections and fill them*/

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

#ifdef MC_SUPPORT_ENABLE
	if (m_isMC) {

		loop->Get(genParticles);
		m_GenParticles->Clear("C");
		for (int ii = 0; ii < genParticles.size(); ii++) {
			((GenParticle*) m_GenParticles->ConstructedAt(ii))->operator=(*(genParticles[ii]));
			m_event->AddAssociatedObject(genParticles[ii]);
		}
		m_event->addCollection(m_GenParticles);

		loop->Get(chits_MCReal);
		m_CaloMCRealHits->Clear("C");
		for (int ii = 0; ii < chits_MCReal.size(); ii++) {
			((CalorimeterMCRealHit*) m_CaloMCRealHits->ConstructedAt(ii))->operator=(*(chits_MCReal[ii]));
			m_event->AddAssociatedObject(chits_MCReal[ii]);
		}
		m_event->addCollection(m_CaloMCRealHits);

		loop->Get(userMCdata);
		for (int ii=0;ii<userMCdata.size();ii++) {
			if (userMCdata[ii]->N==1) {
				m_eventHeader->setWeight(userMCdata[ii]->data);
				if (m_eventHeader->getWeight()==0) {
					m_eventHeader->setWeight(1.);
				}
			}
			/*Adding following lines of code, so that in case of LUND, the weight will be taken from the tenth number,
			 *In other cases, when there is a single number, the event weight is taken from it (N==1)
			 */
			if (userMCdata[ii]->N==10) {
				m_eventHeader->setWeight(userMCdata[ii]->data);
				if (m_eventHeader->getWeight()==0) {
					m_eventHeader->setWeight(1.);
				}
			}
		}

	}
#endif

	/*publish the event*/
	_data.push_back(m_event);

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t TEvent_factory_BDXmini::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t TEvent_factory_BDXmini::fini(void) {
	japp->RootWriteLock();
//	if (m_CaloHits!=0) delete (m_CaloHits);
//	if (m_IntVetoHits!=0) delete (m_IntVetoHits);
//	if (m_ExtVetoHits!=0) delete (m_ExtVetoHits);
	japp->RootUnLock();

	return NOERROR;
}

