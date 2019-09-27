// $Id$
//
//    File: CalorimeterSiPMHit_factory.cc
// Created: Wed Jan 27 16:39:49 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include <JANA/JEvent.h>

//objects we need from the framework
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <DAQ/fa250Mode7Hit.h>
#include <DAQ/fa250WaveboardV1Hit.h>
#include <TT/TranslationTable.h>
//objects we put in the framework
#include <Calorimeter/CalorimeterSiPMHit.h>
#include <Calorimeter/Calorimeterfa250Converter.h>

#include "CalorimeterSiPMHit_factory.h"

//------------------
// init
//------------------
void CalorimeterSiPMHit_factory::Init() {
	VERBOSE = 0;
	japp->GetParameter("CALORIMETER:VERBOSE", VERBOSE);

	m_sipm_gain = new CalibrationHandler<TranslationTable::CALO_Index_t>("/Calorimeter/sipm_gain");

	m_calibration_service = japp->GetService<BDXCalibrationService>();
	m_calibration_service->addCalibration(m_sipm_gain);
}

//------------------
// brun
//------------------
void CalorimeterSiPMHit_factory::ChangeRun(const std::shared_ptr<const JEvent>& event) {

	jout << "CalorimeterSiPMHit_factory::brun new run number: " << event->GetRunNumber() << jendl;
	m_tt = 0;
	event->Get(&m_tt);
	if (m_tt == 0) {
		jerr << " unable to get the TranslationTable from this run!" << jendl;
		throw JException("Unable to get TranslationTable from this run!");
	}

	m_Calorimeterfa250Converter = 0;
	event->Get(&m_Calorimeterfa250Converter);
	if (m_Calorimeterfa250Converter == 0) {
		jerr << " unable to get the Calorimeterfa250Converter!" << jendl;
		throw JException("Unable to get Calorimeterfa250Converter!");
	}

	m_calibration_service->updateCalibration(m_sipm_gain, event->GetRunNumber(), event->GetEventNumber());

	if (VERBOSE > 3) {
		std::map<TranslationTable::CALO_Index_t, std::vector<double> > gainCalibMap;
		std::map<TranslationTable::CALO_Index_t, std::vector<double> >::iterator gainCalibMap_it;
		gainCalibMap = m_sipm_gain->getCalibMap();
		jout << "Got following sipm_gain for run number: " << event->GetRunNumber() << jendl;
		jout << "Rows: " << gainCalibMap.size() << jendl;
		for (gainCalibMap_it = gainCalibMap.begin(); gainCalibMap_it != gainCalibMap.end(); gainCalibMap_it++) {
			jout << gainCalibMap_it->first.sector << " " << gainCalibMap_it->first.x << " " << gainCalibMap_it->first.y << " " << gainCalibMap_it->first.readout << " " << gainCalibMap_it->second.at(0) << jendl;
		}
	}

	jout << "CalorimeterSiPMHit_factory::brun done" << jendl;
}

//------------------
// evnt
//------------------
void CalorimeterSiPMHit_factory::Process(const std::shared_ptr<const JEvent>& event) {

	TranslationTable::ChannelInfo m_channel;
	TranslationTable::csc_t m_csc;
	double m_q_calib;
	CalorimeterSiPMHit *m_CalorimeterSiPMHit = 0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector<const fa250Mode1CalibPedSubHit*> m_fa250Mode1CalibPedSubHit;
	vector<const fa250Mode7Hit*> m_fa250Mode7Hit;
	vector<const fa250Mode1CalibPedSubHit*>::const_iterator it_fa250Mode1CalibPedSubHit;
	vector<const fa250Mode7Hit*>::const_iterator it_fa250Mode7Hit;


	//1b: retrieve objects
	event->Get(m_fa250Mode1CalibPedSubHit);
	event->Get(m_fa250Mode7Hit);


	/*2: Now we have the daq objects, still indexed as "crate-slot-channel"
	 *	 Use the translation table to produce the digitized hit of the inner veto
	 *	 Note that we can produce a single object type here, i.e. VetoIntDigiHit,
	 *	 but we have 2 possible source, mode 1 and mode 7.
	 *	 Therefore, we will use the TranslationTable ONLY to check if this crate-slot-channel
	 *	 combination refers to a Calorimeter hit and, in case, to determine which one, i.e. which ID in the InnerVeto scheme.
	 *	 Then, we will proceed in two different ways.
	 */

	/*First, mode 1*/
	/*Note that in this case we have to integrate the pulse - it is a mode 1 pulse! */
	for (it_fa250Mode1CalibPedSubHit = m_fa250Mode1CalibPedSubHit.begin(); it_fa250Mode1CalibPedSubHit != m_fa250Mode1CalibPedSubHit.end(); it_fa250Mode1CalibPedSubHit++) {

		m_channel = m_tt->getChannelInfo((*it_fa250Mode1CalibPedSubHit)->m_channel);

		if ((m_channel.det_sys == TranslationTable::CALORIMETER) && (m_channel.calorimeter->readout <= 2)) {
			//A.C. do not touch these
			//jout<<eventnumber<<endl;
			m_CalorimeterSiPMHit = m_Calorimeterfa250Converter->convertHit((fa250Hit*) *it_fa250Mode1CalibPedSubHit, m_channel);
			/*Apply phe conversion if possible*/
			m_q_calib = m_sipm_gain->getCalibSingle(*(m_channel.calorimeter));
			if (m_q_calib > 0) {
				m_CalorimeterSiPMHit->Qphe = m_CalorimeterSiPMHit->Qraw / m_q_calib;
			}

			mData.push_back(m_CalorimeterSiPMHit);
		}
	}


	/*Then, mode 7*/
	/*Note that in this case we do not have to integrate the pulse - it is a mode 7 pulse! */
	for (it_fa250Mode7Hit = m_fa250Mode7Hit.begin(); it_fa250Mode7Hit != m_fa250Mode7Hit.end(); it_fa250Mode7Hit++) {
		m_channel = m_tt->getChannelInfo((*it_fa250Mode7Hit)->m_channel);
		if (m_channel.det_sys == TranslationTable::CALORIMETER) {
			//A.C. do not touch these
			m_CalorimeterSiPMHit = m_Calorimeterfa250Converter->convertHit((fa250Hit*) *it_fa250Mode7Hit, m_channel);

			/*Apply phe conversion if possible*/
			m_q_calib = m_sipm_gain->getCalibSingle(*(m_channel.calorimeter));
			if (m_q_calib > 0) {
				m_CalorimeterSiPMHit->Qphe = m_CalorimeterSiPMHit->Qraw / m_q_calib;
			}

			mData.push_back(m_CalorimeterSiPMHit);
		}
	}
}

//------------------
// erun
//------------------
void CalorimeterSiPMHit_factory::EndRun() {

	m_calibration_service->clearCalibration(m_sipm_gain);
}

//------------------
// fini
//------------------
void CalorimeterSiPMHit_factory::Finish() {
}

