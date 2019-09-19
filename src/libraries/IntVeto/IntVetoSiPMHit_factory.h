// $Id$
//
//    File: VetoIntDigiHit_factory.h
// Created: Tue Jan 12 11:52:41 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _VetoIntSIPMHit_factory_
#define _VetoIntSIPMHit_factory_

#include <JANA/JFactoryT.h>
#include "IntVetoSiPMHit.h"
#include <system/CalibrationHandler.h>

class IntVetofa250Converter;




class IntVetoSiPMHit_factory:public JFactoryT<IntVetoSiPMHit>{
	public:
		IntVetoSiPMHit_factory();
		~IntVetoSiPMHit_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun() {}
		void Finish() {}

		const TranslationTable *m_tt;
		const IntVetofa250Converter *m_intVetofa250Converter;

		CalibrationHandler<TranslationTable::INT_VETO_Index_t> *m_sipm_gain;
		CalibrationHandler<TranslationTable::INT_VETO_Index_t> *m_sipm_ampl;

		int VERBOSE;

};

#endif // _VetoIntDigiHit_factory_

