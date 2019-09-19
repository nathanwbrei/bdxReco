// $Id$
//
//    File: CalorimeterDigiHit_factory.cc
// Created: Wed Jan 27 16:39:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include <JANA/JEvent.h>

//objects we need from the framework
#include <TT/TranslationTable.h>
#include <Calorimeter/CalorimeterSiPMHit.h>
//objects we put in the framework
#include <Calorimeter/CalorimeterDigiHit.h>

#include "CalorimeterDigiHit_factory.h"

void CalorimeterDigiHit_factory::Init()
{
}

void CalorimeterDigiHit_factory::ChangeRun(const std::shared_ptr<const JEvent>& event)
{
}

void CalorimeterDigiHit_factory::Process(const std::shared_ptr<const JEvent>& event)
{
	TranslationTable::CALO_Index_t m_channel;
	CalorimeterDigiHit *m_CalorimeterDigiHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const CalorimeterSiPMHit*> m_CalorimeterSiPMHits;
	vector <const CalorimeterSiPMHit*>::const_iterator it;
	const CalorimeterSiPMHit*  m_CalorimeterSiPMHit;
	//1b: retrieve CalorimeterSiPMHit objects
	event->Get(m_CalorimeterSiPMHits);

	for (it=m_CalorimeterSiPMHits.begin();it!=m_CalorimeterSiPMHits.end();it++){
		 m_CalorimeterSiPMHit=(*it);
		 m_CalorimeterDigiHit=new CalorimeterDigiHit();
		 m_CalorimeterDigiHit->m_channel=*(m_CalorimeterSiPMHit->m_channel.calorimeter);
		 m_CalorimeterDigiHit->Q=m_CalorimeterSiPMHit->Qphe;
		 m_CalorimeterDigiHit->T=m_CalorimeterSiPMHit->T;
		 m_CalorimeterDigiHit->A=m_CalorimeterSiPMHit->A;
		 m_CalorimeterDigiHit->pedMean=m_CalorimeterSiPMHit->pedMean;
		 m_CalorimeterDigiHit->pedRMS=m_CalorimeterSiPMHit->pedRMS;
		 m_CalorimeterDigiHit->RMSflag=m_CalorimeterSiPMHit->RMSflag;
		 m_CalorimeterDigiHit->type=m_CalorimeterSiPMHit->type;
		 m_CalorimeterDigiHit->AddAssociatedObject(m_CalorimeterSiPMHit);
		 Insert(m_CalorimeterDigiHit);
	}
}


