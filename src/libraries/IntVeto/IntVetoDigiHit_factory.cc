// $Id$
//
//    File: IntVetoDigiHit_factory.cc
// Created: Wed Jan 20 16:42:38 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
#include <map>
using namespace std;

#include "IntVetoDigiHit_factory.h"

#include <IntVeto/IntVetoSiPMHit.h>
#include <TT/TranslationTable.h>
#include <JANA/JEvent.h>


//------------------
// init
//------------------
void IntVetoDigiHit_factory::Init()
{
}

//------------------
// brun
//------------------
void IntVetoDigiHit_factory::ChangeRun(const std::shared_ptr<const JEvent>& event)
{

	jout<<"IntVetoDigiHit_factory::brun new run number: "<<event->GetRunNumber()<<jendl;
	m_tt=0;
	event->Get(&m_tt);
	if (m_tt==0){
		jerr<<" unable to get the TranslationTable from this run!"<<jendl;
		throw JException(" unable to get the TranslationTable from this run!");
	}
}

//------------------
// evnt
//------------------
void IntVetoDigiHit_factory::Process(const std::shared_ptr<const JEvent>& event)
{

	TranslationTable::INT_VETO_Index_t m_channel;
	IntVetoDigiHit *m_IntVetoDigiHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const IntVetoSiPMHit*> m_IntVetoSiPMHits;
	vector <const IntVetoSiPMHit*>::const_iterator it;
	const IntVetoSiPMHit* m_IntVetoSiPMHit;
	//1b: retrieve IntVetoSiPMHit objects
	event->Get(m_IntVetoSiPMHits);



	for (it=m_IntVetoSiPMHits.begin(); it != m_IntVetoSiPMHits.end() ; it++){
		m_IntVetoSiPMHit=*it;
		m_channel = *(m_IntVetoSiPMHit->m_channel.int_veto);

		m_IntVetoDigiHit=new IntVetoDigiHit;
		m_IntVetoDigiHit->m_channel=m_channel;
		m_IntVetoDigiHit->Qphe=m_IntVetoSiPMHit->Qphe;
		m_IntVetoDigiHit->Qraw=m_IntVetoSiPMHit->Qraw;
		m_IntVetoDigiHit->A=m_IntVetoSiPMHit->Araw;
		m_IntVetoDigiHit->T=m_IntVetoSiPMHit->T;

		m_IntVetoDigiHit->pedMean=m_IntVetoSiPMHit->pedMean;
		m_IntVetoDigiHit->pedRMS=m_IntVetoSiPMHit->pedRMS;
		m_IntVetoDigiHit->RMSflag=m_IntVetoSiPMHit->RMSflag;

		m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoSiPMHit);

		mData.push_back(m_IntVetoDigiHit); //publish it
	}
}

