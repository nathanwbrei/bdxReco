// $Id$
//
//    File: ExtVetoDigiHit_factory.cc
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include <Paddles/PaddlesDigiHit_factory.h>
#include <Paddles/PaddlesPMTHit.h>

#include <TT/TranslationTable.h>

//------------------
// init
//------------------
void PaddlesDigiHit_factory::Init()
{
}

//------------------
// brun
//------------------
void PaddlesDigiHit_factory::ChangeRun(const std::shared_ptr<const JEvent>& event)
{
	jout<<"PaddlesDigiHit_factory::brun new run number: "<<runnumber<<endl;
	m_tt=0;
	eventLoop->GetSingle(m_tt);
	if (m_tt==0){
		jerr<<" unable to get the TranslationTable from this run!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}
}

//------------------
// evnt
//------------------
void PaddlesDigiHit_factory::Process(const std::shared_ptr<const JEvent>& event)
{

	vector <const PaddlesPMTHit *> m_data;
	vector <const PaddlesPMTHit *>::const_iterator m_it;

	loop->Get(m_data);

	/*Create here the digi hit from the PMT hit*/
	PaddlesDigiHit *m_PaddlesDigiHit=0;

	for (m_it=m_data.begin();m_it!=m_data.end();m_it++){
		m_PaddlesDigiHit=new PaddlesDigiHit;

		/*For now, very dummy!*/
		m_PaddlesDigiHit->m_channel=*((*m_it)->m_channel.paddles);
//		m_PaddlesDigiHit->m_channel.readout=0; //This is a physical object
		m_PaddlesDigiHit->Q=(*m_it)->Q;
		m_PaddlesDigiHit->T=(*m_it)->T;
		m_PaddlesDigiHit->PaddlesPMTHit_id=(*m_it)->id;

		m_PaddlesDigiHit->AddAssociatedObject(*m_it);
		_data.push_back(m_PaddlesDigiHit);
	}
}

