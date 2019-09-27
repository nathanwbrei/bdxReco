// $Id$
//
//    File: CalorimeterSiPMHit_factory.h
// Created: Wed Jan 27 16:39:49 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterSiPMHit_factory_
#define _CalorimeterSiPMHit_factory_

#include <JANA/JFactoryT.h>
#include "CalorimeterSiPMHit.h"
#include <system/CalibrationHandler.h>
#include <system/BDXCalibrationService.h>
#include <TT/TranslationTable.h>
class Calorimeterfa250Converter;

class CalorimeterSiPMHit_factory:public JFactoryT<CalorimeterSiPMHit>{
	public:
		CalorimeterSiPMHit_factory():m_sipm_gain(0),m_tt(0),m_Calorimeterfa250Converter(0),VERBOSE(0){};
		~CalorimeterSiPMHit_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();

		const TranslationTable *m_tt;
		const Calorimeterfa250Converter *m_Calorimeterfa250Converter;

		CalibrationHandler<TranslationTable::CALO_Index_t> *m_sipm_gain;
	    std::shared_ptr<BDXCalibrationService> m_calibration_service;

		int VERBOSE;
};

#endif // _CalorimeterSiPMHit_factory_

