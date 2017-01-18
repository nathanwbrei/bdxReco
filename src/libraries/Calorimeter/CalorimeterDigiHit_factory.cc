// $Id$
//
//    File: CalorimeterDigiHit_factory.cc
// Created: Wed Jan 27 16:39:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;


//objects we need from the framework
#include <TT/TranslationTable.h>
#include <Calorimeter/CalorimeterSiPMHit.h>
//objects we put in the framework
#include <Calorimeter/CalorimeterDigiHit.h>

#include "CalorimeterDigiHit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t CalorimeterDigiHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterDigiHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{



	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterDigiHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{


	TranslationTable::CALO_Index_t m_channel;
	CalorimeterDigiHit *m_CalorimeterDigiHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const CalorimeterSiPMHit*> m_CalorimeterSiPMHit;
	vector <const CalorimeterSiPMHit*>::const_iterator it;

	//1b: retrieve CalorimeterSiPMHit objects
	loop->Get(m_CalorimeterSiPMHit);



	return NOERROR;
}



//------------------
// erun
//------------------
jerror_t CalorimeterDigiHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CalorimeterDigiHit_factory::fini(void)
{
	return NOERROR;
}

