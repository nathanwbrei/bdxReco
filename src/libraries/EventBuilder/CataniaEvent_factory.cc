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

#include <Calorimeter/CalorimeterHit.h>
#include <IntVeto/IntVetoHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <Paddles/PaddlesHit.h>


using namespace jana;

//------------------
// init
//------------------
jerror_t CataniaEvent_factory::init(void)
{


	m_EC2_cut=20; //MeV
	m_ExtVeto_timeWindows=250; //ns
	m_IntVeto_timeWindows=250; //ns
	gPARMS->SetDefaultParameter("CATANIAEVENT:EC2_CUT",m_EC2_cut,"Cut on MPPC#2 energy in MeV");
	gPARMS->SetDefaultParameter("CATANIAEVENT:EXTVETO_TIME_WINDOW",m_ExtVeto_timeWindows,"ExtVeto time window (ns)");
	gPARMS->SetDefaultParameter("CATANIAEVENT:INTVETO_TIME_WINDOW",m_ExtVeto_timeWindows,"IntVeto time window (ns)");



	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CataniaEvent_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CataniaEvent_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{



	vector <const CalorimeterHit*> chits;
	vector <const CalorimeterHit*>::const_iterator chits_it;

	vector <const IntVetoHit*> ivhits;
	vector <const IntVetoHit*>::const_iterator ivhits_it;

	vector <const ExtVetoHit*> evhits;
	vector <const ExtVetoHit*>::const_iterator evhits_it;


	vector <const PaddlesHit*> phits;
	vector <const PaddlesHit*>::const_iterator phits_it;

	loop->Get(chits);
	loop->Get(ivhits);
	loop->Get(evhits);
	loop->Get(phits);


	CataniaEvent *m_event=new CataniaEvent;
	m_event->E=0;
	m_event->T=0;
	m_event->flag_RMS=false;
	double E1,E2,T1,T2,dT;
	bool flag1,flag2;


	for (chits_it=chits.begin();chits_it!=chits.end();chits_it++){
		const CalorimeterHit *hit=(*chits_it);
		for (int ihit=0;ihit<hit->m_data.size();ihit++){
			switch (hit->m_data[ihit].readout){
			case 1:
				T1=hit->m_data[ihit].T;
				E1=hit->m_data[ihit].E;
				flag1=hit->m_data[ihit].good_ped_RMS;
				break;
			case 2:
				T2=hit->m_data[ihit].T;
				E2=hit->m_data[ihit].E;
				flag2=hit->m_data[ihit].good_ped_RMS;
				break;
			}
		}
	}
	if (E2<=m_EC2_cut){
		m_event->E=E2;
		m_event->T=T2;
		m_event->flag_RMS=flag2;
	}
	else{
		m_event->E=E1;
		m_event->T=T1;
		m_event->flag_RMS=flag1;
	}

	/*Now loop on external veto hits*/
	m_event->nExtVetoHits=0;
	m_event->nExtVetoHitsCoincidence=0;
	for (evhits_it=evhits.begin();evhits_it!=evhits.end();evhits_it++){
		const ExtVetoHit *hit=(*evhits_it);
		if (hit->T<0) continue; //The ExtVeto condition for a "good" hit
		else{
			m_event->nExtVetoHits++;
			dT=fabs(hit->T-m_event->T);
			if (dT<m_ExtVeto_timeWindows){
				m_event->nExtVetoHitsCoincidence++;
				m_event->AddAssociatedObject(hit);
			}
		}
	}
	/*Now loop on inner veto hits*/
	m_event->nIntVetoHits=0;
	m_event->nIntVetoHitsCoincidence=0;
	for (ivhits_it=ivhits.begin();ivhits_it!=ivhits.end();ivhits_it++){
		const IntVetoHit *hit=(*ivhits_it);
		if (hit->T<0) continue; //The IntVeto condition for a "good" hit
		else{
			m_event->nIntVetoHits++;
			dT=fabs(hit->T-m_event->T);
			if (dT<m_IntVeto_timeWindows){
				m_event->nIntVetoHitsCoincidence++;
				m_event->AddAssociatedObject(hit);
			}
		}
	}

	/*Now loop on paddles*/
	for (phits_it=phits.begin();phits_it!=phits.end();phits_it++){
		const PaddlesHit *phit=(*phits_it);
		switch (phit->m_channel.id){
		case (0):
				m_event->Ep1=phit->E;
		break;
		case (1):
				m_event->Ep2=phit->E;
		break;
		}
	}
	_data.push_back(m_event);

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CataniaEvent_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CataniaEvent_factory::fini(void)
{
	return NOERROR;
}

