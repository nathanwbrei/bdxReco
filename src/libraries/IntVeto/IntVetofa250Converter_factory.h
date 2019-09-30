// $Id$
//
//    File: IntVetofa250Converter_factory.h
// Created: Mon Jan 25 17:02:27 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetofa250Converter_factory_
#define _IntVetofa250Converter_factory_

#include "IntVetofa250Converter.h"
#include <system/BDXCalibrationService.h>
#include <JANA/JFactoryT.h>

class IntVetofa250Converter_factory:public JFactoryT<IntVetofa250Converter>{
	public:
		IntVetofa250Converter_factory();
		~IntVetofa250Converter_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();

		int m_isFirstCallToBrun;
		IntVetofa250Converter *m_intVetofa250Converter;

		double m_minTot,m_thr;
		int m_NSB,m_NSA,m_NPED;
		double m_RMSTHRscale;

		std::shared_ptr<BDXCalibrationService> m_calibration_service;
};

#endif // _IntVetofa250Converter_factory_

