// $Id$
//
//    File: CalorimeterHit_factory.h
// Created: Wed Jan 27 16:40:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterHit_factory_
#define _CalorimeterHit_factory_

#include <JANA/JFactoryT.h>
#include "CalorimeterHit.h"
#include <system/CalibrationHandler.h>
#include <system/BDXCalibrationService.h>

class TranslationTable;

class CalorimeterDigiHit;

class CalorimeterHit_factory:public JFactoryT<CalorimeterHit>{
	public:
		CalorimeterHit_factory();
		~CalorimeterHit_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();

		int isMC;
		const TranslationTable *m_tt;

		int m_THR_singleReadout;
		int m_THR_multipleReadout;
		int m_N_multipleReadout;
		int VERBOSE;

		std::shared_ptr<BDXCalibrationService> m_calibration_service;
		CalibrationHandler<TranslationTable::CALO_Index_t> *m_ene;

		TranslationTable::CALO_Index_t m_channel;
		std::map<TranslationTable::CALO_Index_t,vector <const CalorimeterDigiHit*>> m_map;
		std::map<TranslationTable::CALO_Index_t,vector <const CalorimeterDigiHit*>>::iterator m_map_it;
};

#endif // _CalorimeterHit_factory_

