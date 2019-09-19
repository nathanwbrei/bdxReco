// $Id$
//
//    File: ExtVetoHit_factory.h
// Created: Wed Jan 13 21:06:44 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoHit_factory_
#define _ExtVetoHit_factory_

#include <JANA/JFactoryT.h>
#include "ExtVetoHit.h"
#include <system/CalibrationHandler.h>

class ExtVetoHit_factory:public JFactoryT<ExtVetoHit>{
	public:
	//	ExtVetoHit_factory():m_tt(0),m_ENE_gain(0){isMC=0;};
	ExtVetoHit_factory();
	~ExtVetoHit_factory(){};

	private:
		int isMC;
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();


		double m_THR;

		const TranslationTable *m_tt;
		CalibrationHandler<TranslationTable::EXT_VETO_Index_t> *m_ENE_gain;
		TranslationTable::EXT_VETO_Index_t m_channel;
		std::map<TranslationTable::EXT_VETO_Index_t,vector <const ExtVetoDigiHit*>> m_map;
		std::map<TranslationTable::EXT_VETO_Index_t,vector <const ExtVetoDigiHit*>>::iterator m_map_it;

};

#endif // _ExtVetoHit_factory_

