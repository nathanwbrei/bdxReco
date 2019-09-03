// $Id$
//
//    File: PaddlesDigiHit_factory_MC.cc
// Created: Tue Mar  8 13:31:20 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;


#include <MC/PaddlesMCHit.h>
#include <Paddles/PaddlesDigiHit.h>


#include "PaddlesDigiHit_factory_MC.h"

//------------------
// init
//------------------
jerror_t PaddlesDigiHit_factory_MC::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t PaddlesDigiHit_factory_MC::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t PaddlesDigiHit_factory_MC::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	PaddlesDigiHit *m_PaddlesDigiHit=0;


	const PaddlesMCHit *m_PaddlesMCHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const PaddlesMCHit*> m_PaddlesMCHits;
	vector <const PaddlesMCHit*>::const_iterator it;

	//1b: retrieve PaddlesSiPMHit objects
	loop->Get(m_PaddlesMCHits);

	m_map.clear();

	for (it=m_PaddlesMCHits.begin();it!=m_PaddlesMCHits.end();it++){
		m_PaddlesMCHit = (*it);
		m_map_it=m_map.find(std::make_pair(m_PaddlesMCHit->sector,m_PaddlesMCHit->channel));
		if (m_map_it==m_map.end()){
			m_PaddlesDigiHit=new PaddlesDigiHit;
			/*Here comes the annoying routine that, given the geometry Marco used in the MC, in particular the indexing scheme,
			 * returns it to the scheme I am using in the reconstruction. This was chacked by Andrea and Luca*/
			if (m_PaddlesMCHit->channel==1) m_PaddlesDigiHit->m_channel.id=1;
			else if (m_PaddlesMCHit->channel==2) m_PaddlesDigiHit->m_channel.id=0;
			m_PaddlesDigiHit->Q=m_PaddlesMCHit->adc;
			m_PaddlesDigiHit->T=m_PaddlesMCHit->tdc/1000.;  //MC is in ps
			m_PaddlesDigiHit->AddAssociatedObject(m_PaddlesMCHit);
			m_map[std::make_pair(m_PaddlesMCHit->sector,m_PaddlesMCHit->channel)]=m_PaddlesDigiHit;
		}
		else{
			m_PaddlesDigiHit=m_map_it->second;
			m_PaddlesDigiHit->AddAssociatedObject(m_PaddlesMCHit);
			m_PaddlesDigiHit->Q+=m_PaddlesMCHit->adc;
		}
	}


	for (m_map_it=m_map.begin();m_map_it!=m_map.end();m_map_it++){
		m_PaddlesDigiHit=m_map_it->second;
		_data.push_back(m_PaddlesDigiHit);
	}
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t PaddlesDigiHit_factory_MC::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t PaddlesDigiHit_factory_MC::fini(void)
{
	return NOERROR;
}

