// $Id$
//
//    File: CataniaEvent_factory.cc
// Created: Mon Apr 25 23:25:44 CEST 2016
// Creator: celentan (on Linux dhcp-visitor-enr-116-196.slac.stanford.edu 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "CataniaEvent_factory.h"

#include <Calorimeter/CalorimeterDigiHit.h>
#include <Calorimeter/CalorimeterHit.h>
#include <IntVeto/IntVetoHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <Paddles/PaddlesHit.h>
#include <DAQ/eventData.h>


//------------------
// init
//------------------
jerror_t CataniaEvent_factory::init(void) {

	m_EC2_cut = 20; //MeV
	m_ExtVeto_timeWindows = 250; //ns
	m_IntVeto_timeWindows = 250; //ns
	gPARMS->SetDefaultParameter("CATANIAEVENT:EC2_CUT", m_EC2_cut, "Cut on MPPC#2 energy in MeV");
	gPARMS->SetDefaultParameter("CATANIAEVENT:EXTVETO_TIME_WINDOW", m_ExtVeto_timeWindows, "ExtVeto time window (ns)");
	gPARMS->SetDefaultParameter("CATANIAEVENT:INTVETO_TIME_WINDOW", m_IntVeto_timeWindows, "IntVeto time window (ns)");

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CataniaEvent_factory::brun(JEventLoop *eventLoop, int32_t runnumber) {
	gPARMS->GetParameter("MC", m_isMC);
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CataniaEvent_factory::evnt(JEventLoop *loop, uint64_t eventnumber) {

	vector<const CalorimeterDigiHit*> cdigihits;
	vector<const CalorimeterDigiHit*>::const_iterator cdigihits_it;

	vector<const CalorimeterHit*> chits;
	vector<const CalorimeterHit*>::const_iterator chits_it;

	vector<const IntVetoHit*> ivhits;
	vector<const IntVetoHit*>::const_iterator ivhits_it;

	vector<const ExtVetoHit*> evhits;
	vector<const ExtVetoHit*>::const_iterator evhits_it;

	vector<const PaddlesHit*> phits;
	vector<const PaddlesHit*>::const_iterator phits_it;

	const eventData *evData;
	if (m_isMC) {
		loop->Get(cdigihits, "MC");
	} else {
		loop->Get(cdigihits);
	}
	loop->Get(chits);
	loop->Get(ivhits);
	loop->Get(evhits);
	loop->Get(phits);

	if (m_isMC == 0) {
		try {
			loop->GetSingle(evData);
		} catch (unsigned long e) {
			jout << "CataniaEvent_factor no eventData this event: " << eventnumber << endl;
			return NOERROR;
		}
	}
	CataniaEvent *m_event = new CataniaEvent();
	m_event->timestamp = 0;
	m_event->E1 = 0;
	m_event->T1 = 0;
	m_event->E2 = 0;
	m_event->T2 = 0;
	m_event->flag_RMS1 = false;
	m_event->flag_RMS2 = false;
	m_event->flag_RMS = false;
	if (m_isMC == 0) {
		m_event->time = evData->time;
		m_event->tWord = evData->triggerWords[0];
		m_event->eventN = evData->eventN;
		if (m_event->eventN != eventnumber) {
			jerr << "CataniaEvent_factor::Something wrong with event number!" << endl;
		}
		m_event->runN = evData->runN;
	} else {
		m_event->time = 0;
		m_event->tWord = 0;
		m_event->eventN = eventnumber;
		m_event->runN = 0;
	}

	double E1, E2, T1, T2, dT;
	bool flag1, flag2;

	E1 = E2 = 0;
	T1 = T2 = 0;
	flag1 = false;
	flag2 = false;

	/*Use digihits just for timestamp*/
	for (cdigihits_it = cdigihits.begin(); cdigihits_it != cdigihits.end(); cdigihits_it++) {
		const CalorimeterDigiHit *hit = (*cdigihits_it);
		//m_event->timestamp=hit->timestamp; //it is irrelevant to take timestamp from this or from #2, they're the same!
	}

	for (chits_it = chits.begin(); chits_it != chits.end(); chits_it++) {
		const CalorimeterHit *hit = (*chits_it);
		if ((hit->m_channel.y == 0) && (hit->m_channel.x == 0)) { //this is the first crystal

			E1 = hit->E;
			T1 = hit->T;
			flag1 = hit->RMSflag;
			m_event->flag_RMS1 = flag1;
			m_event->E1 = E1;

		} else if ((hit->m_channel.y == 1) && (hit->m_channel.x == 0)) { //the second crystal, if present
			E2 = hit->E;
			T2 = hit->T;
			flag2 = hit->RMSflag;
			m_event->flag_RMS2 = flag2;
			m_event->E2 = E2;
		}
		m_event->AddAssociatedObject(hit);
	}

	/*
	 if (E2<=m_EC2_cut){
	 m_event->E1=E2;
	 m_event->T1=T2;
	 m_event->flag_RMS1=flag2;
	 }
	 else{
	 m_event->E1=E1;
	 m_event->T1=T1;
	 m_event->flag_RMS1=flag1;
	 }*/

	m_event->E = (m_event->E1 + m_event->E2) / 2;
	m_event->T = (m_event->T1 * m_event->E1 + m_event->T2 * m_event->E2) / ((m_event->E1 + m_event->E2));
	m_event->flag_RMS = (m_event->flag_RMS1) && (m_event->flag_RMS2);

	/*Now loop on external veto hits*/
	m_event->nExtVetoHits = 0;
	m_event->nExtVetoHitsCoincidence = 0;
	for (evhits_it = evhits.begin(); evhits_it != evhits.end(); evhits_it++) {
		const ExtVetoHit *hit = (*evhits_it);
		if (hit->T < 0) continue; //The ExtVeto condition for a "good" hit
		else {
			m_event->nExtVetoHits++;
			m_event->vExtVetoHits.push_back(hit->m_channel);
			m_event->vExtVetoHitsT.push_back(hit->T);
			m_event->vExtVetoHitsE.push_back(hit->E);
			dT = fabs(hit->T - m_event->T);
			if (dT < m_ExtVeto_timeWindows) {
				m_event->nExtVetoHitsCoincidence++;
				m_event->vExtVetoHitsIsInCoincidence.push_back(true);
			} else {
				m_event->vExtVetoHitsIsInCoincidence.push_back(false);
			}
			m_event->AddAssociatedObject(hit);
		}
	}
	/*Now loop on inner veto hits*/
	m_event->nIntVetoHits = 0;
	m_event->nIntVetoHitsCoincidence = 0;
	for (ivhits_it = ivhits.begin(); ivhits_it != ivhits.end(); ivhits_it++) {
		const IntVetoHit *hit = (*ivhits_it);
		if (hit->T < 0) continue; //The IntVeto condition for a "good" hit
		else {
			m_event->nIntVetoHits++;
			m_event->vIntVetoHits.push_back(hit->m_channel);
			m_event->vIntVetoHitsT.push_back(hit->T);
			m_event->vIntVetoHitsQ.push_back(hit->Q);
			dT = fabs(hit->T - m_event->T);
			if (dT < m_IntVeto_timeWindows) {
				m_event->nIntVetoHitsCoincidence++;
				m_event->vIntVetoHitsIsInCoincidence.push_back(true);
			} else {
				m_event->vIntVetoHitsIsInCoincidence.push_back(false);
			}
			m_event->AddAssociatedObject(hit);

		}
	}

	/*Now loop on paddles*/
	m_event->Ep1 = 0;
	m_event->Ep2 = 0;
	for (phits_it = phits.begin(); phits_it != phits.end(); phits_it++) {
		const PaddlesHit *phit = (*phits_it);
		switch (phit->m_channel.id) {
		case (0):
			m_event->Ep1 = phit->E;
			break;
		case (1):
			m_event->Ep2 = phit->E;
			break;
		}
	}
	_data.push_back(m_event);

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CataniaEvent_factory::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CataniaEvent_factory::fini(void) {
	return NOERROR;
}

