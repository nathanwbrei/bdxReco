// $Id$
//
//    File: CalorimeterHit_factory.cc
// Created: Wed Jan 27 16:40:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "CalorimeterHit_factory.h"

#include <JANA/JEvent.h>
//objects we need from the framework
#include <TT/TranslationTable.h>
#include <Calorimeter/CalorimeterDigiHit.h>
//objects we put in the framework
#include <Calorimeter/CalorimeterHit.h>


CalorimeterHit_factory::CalorimeterHit_factory() :
		m_ene(0), m_tt(0) {
	VERBOSE = 0;
	isMC = 0;
	m_THR_singleReadout = 5;
	m_THR_multipleReadout = 3;
	m_N_multipleReadout = 2;

	japp->SetDefaultParameter("CALORIMETER:HIT_THR_SINGLE", m_THR_singleReadout, "Threshold in phe (charge) for a detector with single readout");
	japp->SetDefaultParameter("CALORIMETER:HIT_THR_MULTI", m_THR_multipleReadout, "Threshold in phe (charge) for a detector with multi readout");
	japp->SetDefaultParameter("CALORIMETER:HIT_N_MULTI", m_N_multipleReadout, "Multiplicity for a detector with multi readout");
	japp->GetParameter("CALORIMETER:VERBOSE", VERBOSE);

}

//------------------
// init
//------------------
void CalorimeterHit_factory::Init() {

	m_ene = new CalibrationHandler<TranslationTable::CALO_Index_t>("/Calorimeter/Ene");
	this->mapCalibrationHandler(m_ene);

	japp->GetParameter("MC", isMC);
}

//------------------
// brun
//------------------
void CalorimeterHit_factory::ChangeRun(const std::shared_ptr<const JEvent>& event) {

	this->updateCalibrationHandler(m_ene, eventLoop);

	if (VERBOSE > 3) {
		std::map<TranslationTable::CALO_Index_t, std::vector<double> > gainCalibMap;
		std::map<TranslationTable::CALO_Index_t, std::vector<double> >::iterator gainCalibMap_it;
		gainCalibMap = m_ene->getCalibMap();
		jout << "Got following ene for run number: " << event->GetRunNumber() << jendl;
		jout << "Rows: " << gainCalibMap.size() << jendl;
		for (gainCalibMap_it = gainCalibMap.begin(); gainCalibMap_it != gainCalibMap.end(); gainCalibMap_it++) {
			jout << gainCalibMap_it->first.sector << " " << gainCalibMap_it->first.x << " " << gainCalibMap_it->first.y << " " << gainCalibMap_it->first.readout << " " << gainCalibMap_it->second[0] << " " << gainCalibMap_it->second[1] << jendl;
		}
	}
}

//------------------
// evnt
//------------------
void CalorimeterHit_factory::Process(const std::shared_ptr<const JEvent>& event) {

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector<const CalorimeterDigiHit*> m_CalorimeterDigiHits;
	vector<const CalorimeterDigiHit*>::const_iterator it;

	const CalorimeterDigiHit *m_CalorimeterDigiHit;
	CalorimeterHit *m_CalorimeterHit = 0;

	int countOk;
	double Q, T, Qtot, Qmax, Tmax;
	double gain, ped;

	if (isMC) {
		event->Get(m_CalorimeterDigiHits, "MC");
	} else {
		event->Get(m_CalorimeterDigiHits);
	}
	/*Do the matching
	 */
	m_map.clear();
	for (it = m_CalorimeterDigiHits.begin(); it != m_CalorimeterDigiHits.end(); it++) {
		m_channel = ((*it)->m_channel);
		/*a.c. fix for crs x=0 y=0 first catania Proto*/
		if (m_channel.readout != 1) continue;
		m_channel.readout = 0;
		m_map[m_channel].push_back(*it);

	}

	/*Now the map is full of all the hits in different active elements of calorimeter, i.e. with different identifiers, BUT readout, that maps the sipm hits.
	 * Each hit has a reference to the digi hits that made it
	 */
	vector<const CalorimeterDigiHit*> m_CalorimeterDigiHit_tmp;
	for (m_map_it = m_map.begin(); m_map_it != m_map.end(); m_map_it++) {

		m_CalorimeterDigiHit_tmp = m_map_it->second;

		//Do some processing
		if (m_CalorimeterDigiHit_tmp.size() == 1) { //single-ch readout
			m_CalorimeterDigiHit = m_CalorimeterDigiHit_tmp[0];
			Q = m_CalorimeterDigiHit->Q;
			T = m_CalorimeterDigiHit->T;
			if (Q > m_THR_singleReadout) {
				m_CalorimeterHit = new CalorimeterHit();
				m_CalorimeterHit->m_channel = m_CalorimeterDigiHit->m_channel;
				m_CalorimeterHit->m_channel.readout = 0;
				m_CalorimeterHit->T = T;
				m_CalorimeterHit->Eraw = Q;
				m_CalorimeterHit->A = m_CalorimeterDigiHit->A;
				m_CalorimeterHit->RMSflag = m_CalorimeterDigiHit->RMSflag;

				/*Try to calibrate in energy and ped-sub*/
				gain = m_ene->getCalib(m_CalorimeterHit->m_channel)[0];
				ped = m_ene->getCalib(m_CalorimeterHit->m_channel)[1];
				m_CalorimeterHit->E = (Q - ped);
				if (gain != 0) {
					m_CalorimeterHit->E /= gain;
				}
				m_CalorimeterHit->AddAssociatedObject(m_CalorimeterDigiHit);
				Insert(m_CalorimeterHit); //publish it
			}
		}
		/*Multiple readout object:
		 *
		 *  This is the case of crs x=0 y=0 first catania Proto
		 *  This is also the case of JLabFlux0 crs
		 *  */
		else if (m_CalorimeterDigiHit_tmp.size() >= 2) {
			countOk = 0;
			Qtot = 0;
			Qmax = -9999;
			for (int idigi = 0; idigi < m_CalorimeterDigiHit_tmp.size(); idigi++) {
				m_CalorimeterDigiHit = m_CalorimeterDigiHit_tmp[idigi];
				Q = m_CalorimeterDigiHit->Q;
				T = m_CalorimeterDigiHit->T;

				if (Q > m_THR_multipleReadout) {
					countOk++;
					Qtot += Q;
				}
				if (Q > Qmax) {
					Qmax = Q;
					Tmax = T;
				}
			}

			/*At the end of this loop, countOK is the number of counters above thr*/
			if (countOk >= m_N_multipleReadout) {
				m_CalorimeterHit = new CalorimeterHit();
				m_CalorimeterHit->m_channel = m_CalorimeterDigiHit->m_channel;
				m_CalorimeterHit->m_channel.readout = 0;
				m_CalorimeterHit->T = Tmax;

				/*Loop again to associate*/
				for (int idigi = 0; idigi < m_CalorimeterDigiHit_tmp.size(); idigi++) {
					m_CalorimeterDigiHit = m_CalorimeterDigiHit_tmp[idigi];
					m_CalorimeterHit->AddAssociatedObject(m_CalorimeterDigiHit);
				}
				/*Try to calibrate in energy and ped-sub*/
				gain = m_ene->getCalib(m_CalorimeterHit->m_channel)[0];
				ped = m_ene->getCalib(m_CalorimeterHit->m_channel)[1];
				m_CalorimeterHit->E = (Qmax - ped);
				m_CalorimeterHit->Eraw = Qmax;
				m_CalorimeterHit->A = m_CalorimeterDigiHit->A;

				if (gain != 0) {
					m_CalorimeterHit->E /= gain;
				}
				Insert(m_CalorimeterHit);
			}
		}

	}
}

//------------------
// erun
//------------------
void CalorimeterHit_factory::EndRun() {

	this->clearCalibrationHandler(m_ene);
}

//------------------
// fini
//------------------
void CalorimeterHit_factory::Finish() {
}

