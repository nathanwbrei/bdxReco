// $Id$
//
//    File: IntVetoHit_factory.h
// Created: Wed Jan 13 21:08:09 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoHit_factory_
#define _IntVetoHit_factory_

#include <JANA/JFactoryT.h>
#include "IntVetoHit.h"
#include <system/CalibrationHandler.h>

class IntVetoHit_factory:public JFactoryT<IntVetoHit>{
	public:
		IntVetoHit_factory();
		~IntVetoHit_factory(){};


	private:
		int isMC;
		int VERBOSE;
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun() {}
		void Finish() {}


		double m_THR_singleReadout;
		double m_THR_multipleReadout;
		double m_DT_multipleReadout;
		int m_N_multipleReadout;
		int m_hit_bottom_workAround;

		TranslationTable::INT_VETO_Index_t m_channel;
		std::map<TranslationTable::INT_VETO_Index_t,vector <const IntVetoDigiHit*>> m_map;
		std::map<TranslationTable::INT_VETO_Index_t,vector <const IntVetoDigiHit*>>::iterator m_map_it;
};

#endif // _IntVetoHit_factory_

