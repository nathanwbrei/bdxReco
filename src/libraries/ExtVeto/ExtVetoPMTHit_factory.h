// $Id$
//
//    File: ExtVetoDigiHit_factory.h
// Created: Wed Jan 13 21:06:40 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoPMTHit_factory_
#define _ExtVetoPMTHit_factory_

#include <JANA/JFactoryT.h>
#include "ExtVetoPMTHit.h"
#include <system/CalibrationHandler.h>
#include <system/BDXCalibrationService.h>

class TranslationTable;
class ExtVetofa250Converter;
class ExtVetoPMTHit_factory:public JFactoryT<ExtVetoPMTHit>{
	public:
		ExtVetoPMTHit_factory():m_tt(0),m_extVetofa250Converter(0),m_PMT_gain(0),VERBOSE(0){};
		~ExtVetoPMTHit_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();

		const TranslationTable *m_tt;
		const ExtVetofa250Converter *m_extVetofa250Converter;
		CalibrationHandler<TranslationTable::EXT_VETO_Index_t> *m_PMT_gain;
		std::shared_ptr<BDXCalibrationService> m_calibration_service;

		int VERBOSE;



};

#endif // _ExtVetoDigiHit_factory_

