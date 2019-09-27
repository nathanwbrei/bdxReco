// $Id$
//
//    File: CalorimeterMCRealHit_factory.cc
// Created: Wed Feb 22 15:46:36 CET 2017
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include <JANA/JEvent.h>
#include "Calorimeter/CalorimeterDigiHit_factory_MC.h"
#include "CalorimeterMCRealHit_factory.h"
#include "MC/CalorimeterMCHit.h"


void CalorimeterMCRealHit_factory::ChangeRun(const std::shared_ptr<const JEvent>& event) {
	japp->GetParameter("MC", isMC);
}

void CalorimeterMCRealHit_factory::Process(const std::shared_ptr<const JEvent>& event) {

	vector<const CalorimeterMCHit*> m_CalorimeterMCHits;
	vector<const CalorimeterMCHit*>::const_iterator it;

	const CalorimeterMCHit *m_CalorimeterMCHit = 0;

	CalorimeterMCRealHit *m_CalorimeterMCRealHit = 0;

	if (isMC == 0) {
	    throw JException("MC is not available!");
	}

	event->Get(m_CalorimeterMCHits);
	m_map.clear();

	for (it = m_CalorimeterMCHits.begin(); it != m_CalorimeterMCHits.end(); it++) {
		TranslationTable::CALO_Index_t index;
		m_CalorimeterMCHit = *it;

		if (m_CalorimeterMCHit->totEdep<=0) continue; //meaningless

		CalorimeterDigiHit_factory_MC::SetIndex(index, m_CalorimeterMCHit, isMC);

		m_map_it = m_map.find(index);
		if (m_map_it == m_map.end()) {
			m_CalorimeterMCRealHit = new CalorimeterMCRealHit;
			m_CalorimeterMCRealHit->m_channel = index;
			m_CalorimeterMCRealHit->E = m_CalorimeterMCHit->totEdep;
			m_CalorimeterMCRealHit->AddAssociatedObject(m_CalorimeterMCHit);
			m_map[index] = 	m_CalorimeterMCRealHit;
		} else {
			m_CalorimeterMCRealHit=m_map_it->second;
			m_CalorimeterMCRealHit->AddAssociatedObject(m_CalorimeterMCHit);
			m_CalorimeterMCRealHit->E += m_CalorimeterMCHit->totEdep;
		}
	}
	for (m_map_it = m_map.begin(); m_map_it != m_map.end(); m_map_it++) {
		m_CalorimeterMCRealHit = m_map_it->second;
		mData.push_back(m_CalorimeterMCRealHit);
	}
}
