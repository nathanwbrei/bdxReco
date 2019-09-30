// $Id$
//
//    File: IntVetoDigiHit_factory_MC.cc
// Created: Mon Mar  7 12:20:33 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "IntVetoDigiHit_factory_MC.h"

#include <MC/IntVetoMCHit.h>
#include <MC/MCType.h>
#include <IntVeto/IntVetoDigiHit.h>
#include <JANA/JEvent.h>


//------------------
// init
//------------------
void IntVetoDigiHit_factory_MC::Init() {
	japp->GetParameter("MC", m_isMC);
}

//------------------
// brun
//------------------
void IntVetoDigiHit_factory_MC::ChangeRun(const std::shared_ptr<const JEvent>& event) {
}

//------------------
// evnt
//------------------
void IntVetoDigiHit_factory_MC::Process(const std::shared_ptr<const JEvent>& event) {

	IntVetoDigiHit *m_IntVetoDigiHit = 0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector<const IntVetoMCHit*> m_IntVetoMCHits;
	vector<const IntVetoMCHit*>::const_iterator it;
	const IntVetoMCHit *m_IntVetoMCHit = 0;

	//1b: retrieve IntVetoSiPMHit objects
	event->Get(m_IntVetoMCHits);

	m_map.clear();
	for (it = m_IntVetoMCHits.begin(); it != m_IntVetoMCHits.end(); it++) {
		m_IntVetoMCHit = (*it);
		m_channel.sector = m_IntVetoMCHit->sector;
		m_channel.layer = 0;
		if (m_isMC == MCType::CATANIA_V1) {
			m_channel.component = this->getCataniaV1Component(m_IntVetoMCHit->channel);
		} else if (m_isMC == MCType::CATANIA_V2) {
			m_channel.component = this->getCataniaV2Component(m_IntVetoMCHit->channel);
		} else if (m_isMC == MCType::FULL_V1) {
			m_channel.component = this->getFullV1Component(m_IntVetoMCHit->channel);
		} else if (m_isMC == MCType::FULL_V2) {
			m_channel.component = this->getFullV2Component(m_IntVetoMCHit->channel);
		} else if (m_isMC == MCType::JLAB_FLUX_V1) {
			m_channel.component = this->getJLabFluxV1Component(m_IntVetoMCHit->channel);
		} else if (m_isMC == MCType::BDXmini_V1) {
			m_channel.component = this->getBDXminiV1Component(m_IntVetoMCHit->channel);
			if (m_IntVetoMCHit->system == 7) m_channel.layer = 0; //outer veto
			else if (m_IntVetoMCHit->system == 8) m_channel.layer = 1; //inner veto
		}

		m_channel.readout = 0;
		m_map_it = m_map.find(m_channel);

		if (m_map_it == m_map.end()) { /*IntVetoDigiHit was not here. Create a new hit*/

			if (m_isMC == MCType::JLAB_FLUX_V1) {
				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 1;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

			} else if (m_isMC == MCType::BDXmini_V1) { /*BDXminiV1 has 8 sipms all around each veto. Sector is 0, layer 1 is inner, 0 is outer.*/
				switch (m_channel.component) {
				case 9: //the bottom
				case 10: //the top
					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
					break;

				case 1: //the full layer

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.component = 1;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.component = 2;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc2;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc2 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.component = 3;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc3;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc3 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.component = 4;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc4;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc4 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.component = 5;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc5;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc5 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.component = 6;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc6;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc6 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.component = 7;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc7;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc7 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.component = 8;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc8;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc8 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
					break;
				}
			}

			else {/*create the different digi-hits for other cases*/
				switch (m_channel.component) {
				case (0): //top
				{
					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 1;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 2;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc2;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc2 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 3;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc3;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc3 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 4;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc4;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc4 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
					break;
				}
				case (1): //left
				case (2): //right
				{
					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 1;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc4;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc4 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 2;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc3;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc3 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 3;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc2;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc2 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 4;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
					break;
				}
				case (3): {
					if (m_isMC == MCType::CATANIA_V1) {
						m_IntVetoDigiHit = new IntVetoDigiHit;
						m_IntVetoDigiHit->m_channel = m_channel;
						m_IntVetoDigiHit->m_channel.readout = 1;
						m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
						m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
						m_IntVetoDigiHit->A = 0;
						m_IntVetoDigiHit->RMSflag = true;
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

						m_IntVetoDigiHit = new IntVetoDigiHit;
						m_IntVetoDigiHit->m_channel = m_channel;
						m_IntVetoDigiHit->m_channel.readout = 2;
						m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc4;
						m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc4 / 1000.; //MC is in ps
						m_IntVetoDigiHit->A = 0;
						m_IntVetoDigiHit->RMSflag = true;
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

						m_IntVetoDigiHit = new IntVetoDigiHit;
						m_IntVetoDigiHit->m_channel = m_channel;
						m_IntVetoDigiHit->m_channel.readout = 3;
						m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc3;
						m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc3 / 1000.; //MC is in ps
						m_IntVetoDigiHit->A = 0;
						m_IntVetoDigiHit->RMSflag = true;
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

						m_IntVetoDigiHit = new IntVetoDigiHit;
						m_IntVetoDigiHit->m_channel = m_channel;
						m_IntVetoDigiHit->m_channel.readout = 4;
						m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc2;
						m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc2 / 1000.; //MC is in ps
						m_IntVetoDigiHit->A = 0;
						m_IntVetoDigiHit->RMSflag = true;
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					} else {
						m_IntVetoDigiHit = new IntVetoDigiHit;
						m_IntVetoDigiHit->m_channel = m_channel;
						m_IntVetoDigiHit->m_channel.readout = 1;
						m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
						m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
						m_IntVetoDigiHit->A = 0;
						m_IntVetoDigiHit->RMSflag = true;
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

						m_IntVetoDigiHit = new IntVetoDigiHit;
						m_IntVetoDigiHit->m_channel = m_channel;
						m_IntVetoDigiHit->m_channel.readout = 2;
						m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc2;
						m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc2 / 1000.; //MC is in ps
						m_IntVetoDigiHit->A = 0;
						m_IntVetoDigiHit->RMSflag = true;
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

						m_IntVetoDigiHit = new IntVetoDigiHit;
						m_IntVetoDigiHit->m_channel = m_channel;
						m_IntVetoDigiHit->m_channel.readout = 3;
						m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc3;
						m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc3 / 1000.; //MC is in ps
						m_IntVetoDigiHit->A = 0;
						m_IntVetoDigiHit->RMSflag = true;
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

						m_IntVetoDigiHit = new IntVetoDigiHit;
						m_IntVetoDigiHit->m_channel = m_channel;
						m_IntVetoDigiHit->m_channel.readout = 4;
						m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc4;
						m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc4 / 1000.; //MC is in ps
						m_IntVetoDigiHit->A = 0;
						m_IntVetoDigiHit->RMSflag = true;
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
					}
					break;
				}
				case (4): //upstream
				case (5): //downstream
				{
					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 1;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
					break;
				}
				}
			}
		} else { /*There is already the hit. Sum the charges*/
			if (m_isMC == MCType::JLAB_FLUX_V1) {
				m_IntVetoDigiHit = m_map_it->second[0];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;

			} else if (m_isMC == MCType::BDXmini_V1) {
				switch (m_channel.component) {
				case 9:
				case 10:
					m_IntVetoDigiHit = m_map_it->second[0];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
					break;
				case 1:
					m_IntVetoDigiHit = m_map_it->second[0];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;

					m_IntVetoDigiHit = m_map_it->second[1];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc2;

					m_IntVetoDigiHit = m_map_it->second[2];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc3;

					m_IntVetoDigiHit = m_map_it->second[3];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc4;

					m_IntVetoDigiHit = m_map_it->second[4];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc5;

					m_IntVetoDigiHit = m_map_it->second[5];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc6;

					m_IntVetoDigiHit = m_map_it->second[6];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc7;

					m_IntVetoDigiHit = m_map_it->second[7];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc8;

					break;
				}
			}

			else {
				switch (m_channel.component) {
				case (0): //top

				{
					m_IntVetoDigiHit = m_map_it->second[0];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit = m_map_it->second[1];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc2;
					m_IntVetoDigiHit = m_map_it->second[2];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc3;
					m_IntVetoDigiHit = m_map_it->second[3];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc4;
					break;
				}
				case (1): //left
				case (2): //right
				{
					m_IntVetoDigiHit = m_map_it->second[0];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc4;
					m_IntVetoDigiHit = m_map_it->second[1];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc3;
					m_IntVetoDigiHit = m_map_it->second[2];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc2;
					m_IntVetoDigiHit = m_map_it->second[3];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
					break;
				}
				case (3): //bottom
				{
					if (m_isMC == MCType::CATANIA_V1) {
						m_IntVetoDigiHit = m_map_it->second[0];
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
						m_IntVetoDigiHit = m_map_it->second[1];
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc4;
						m_IntVetoDigiHit = m_map_it->second[2];
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc3;
						m_IntVetoDigiHit = m_map_it->second[3];
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc2;
						break;
					} else {
						m_IntVetoDigiHit = m_map_it->second[0];
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
						m_IntVetoDigiHit = m_map_it->second[1];
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc2;
						m_IntVetoDigiHit = m_map_it->second[2];
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc3;
						m_IntVetoDigiHit = m_map_it->second[3];
						m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
						m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc4;
						break;
					}
				}
				case (4): //upstream
				case (5): //downstream
				{
					m_IntVetoDigiHit = m_map_it->second[0];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
				}
				}
			}
		}
	} //end loop on MC hits
	for (m_map_it = m_map.begin(); m_map_it != m_map.end(); m_map_it++) {
		for (int idigi = 0; idigi < m_map_it->second.size(); idigi++) {
			m_IntVetoDigiHit = m_map_it->second[idigi];
			mData.push_back(m_IntVetoDigiHit);
		}
	}
}


int IntVetoDigiHit_factory_MC::getCataniaV1Component(int MCchannel) {
	int component = -1;
	switch (MCchannel) {
	case (1): //top
		component = 0;
		break;
	case (2): //bottom
		component = 3;
		break;
	case (3):  //upstream
		component = 4;
		break;
	case (4): //downstream
		component = 5;
		break;
	case (5): //right
		component = 2;
		break;
	case (6): //left
		component = 1;
		break;
	}
	return component;
}

int IntVetoDigiHit_factory_MC::getCataniaV2Component(int MCchannel) {
	int component = -1;
	switch (MCchannel) {
	case (1): //top
		component = 0;
		break;
	case (2): //bottom
		component = 3;
		break;
	case (3):  //upstream
		component = 4;
		break;
	case (4): //downstream
		component = 5;
		break;
		/* A.C. 10/8/2017.
		 * THIS IS CORRECT, in the prototype, looking from the amplifiers's side, the IV plate on the right is the LEFT one, with component n.1.
		 * In the simulations, looking from the amplifiers's side, the IV plate on the right is the RIGHT one, with ch=5. It has to have component n.1
		 * And vice-versa for the other side.
		 */
	case (5): //right
		component = 1;
		break;
	case (6): //left
		component = 2;
		break;
	}
	return component;
}

int IntVetoDigiHit_factory_MC::getFullV1Component(int MCchannel) {
	int component = -1;
	switch (MCchannel) {
	case (1): //top
		component = 0;
		break;
	case (2): //bottom
		component = 3;
		break;
	case (3):  //upstream
		component = 4;
		break;
	case (4): //downstream
		component = 5;
		break;
	case (5): //right
		component = 2;
		break;
	case (6): //left
		component = 1;
		break;
	}
	return component;
}

int IntVetoDigiHit_factory_MC::getFullV2Component(int MCchannel) {

	//A.C. to be checked
	int component = -1;
	switch (MCchannel) {
	case (1): //top
		component = 0;
		break;
	case (2): //bottom
		component = 3;
		break;
	case (3):  //upstream
		component = 4;
		break;
	case (4): //downstream
		component = 5;
		break;
	case (5): //right
		component = 2;
		break;
	case (6): //left
		component = 1;
		break;
	}
	return component;
}

int IntVetoDigiHit_factory_MC::getJLabFluxV1Component(int MCchannel) {
	int component = -1;

	component = MCchannel; //A.C. using the same numbering (from 1 to 13) as used by Marco in MC

	return component;
}

int IntVetoDigiHit_factory_MC::getBDXminiV1Component(int MCchannel) {
	int component = -1;

	if (MCchannel == 9) component = 10;      //the TOP
	else if (MCchannel == 10) component = 9; //the BOTTOM
	else if (MCchannel == 1) component = 1;

	return component;
}
