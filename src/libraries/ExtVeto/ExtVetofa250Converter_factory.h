// $Id$
//
//    File: ExtVetofa250Converter_factory.h
// Created: Mon Jan 25 16:28:59 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetofa250Converter_factory_
#define _ExtVetofa250Converter_factory_

#include "ExtVetofa250Converter.h"
#include <system/BDXCalibrationService.h>
#include <JANA/JFactoryT.h>

class ExtVetofa250Converter_factory:public JFactoryT<ExtVetofa250Converter>{
	public:
		ExtVetofa250Converter_factory():m_isFirstCallToBrun(1),m_extVetofa250Converter(0){};
		~ExtVetofa250Converter_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();

		int m_isFirstCallToBrun;
		ExtVetofa250Converter *m_extVetofa250Converter;
		std::shared_ptr<BDXCalibrationService> m_calibration_service;
};

#endif // _ExtVetofa250Converter_factory_

