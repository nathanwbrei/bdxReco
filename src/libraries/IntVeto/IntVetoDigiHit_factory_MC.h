// $Id$
//
//    File: IntVetoDigiHit_factory_MC.h
// Created: Mon Mar  7 12:20:33 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoDigiHit_factory_MC_
#define _IntVetoDigiHit_factory_MC_

#include <JANA/JFactoryT.h>
#include "IntVetoDigiHit.h"
#include <map>
#include <utility>
#include <algorithm>

class IntVetoMCHit;

class IntVetoDigiHit_factory_MC:public JFactoryT<IntVetoDigiHit>{
	public:
		IntVetoDigiHit_factory_MC(){};
		~IntVetoDigiHit_factory_MC(){};
		const char* Tag(void){return "MC";}

		int getCataniaV1Component(int MCchannel);
		int getCataniaV2Component(int MCchannel);
		int getFullV1Component(int MCchannel);
		int getFullV2Component(int MCchannel);
		int getJLabFluxV1Component(int MCchannel);
		int getBDXminiV1Component(int MCchannel);
	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun() {}
		void Finish() {}

		/*This code is here because MC could generate more than 1 hit per sector!*/
		/*The key is the way MC is organized: sector - channel (where sector and channel have a different meaning than in the real data!!!)*/
		std::map<TranslationTable::INT_VETO_Index_t,vector<IntVetoDigiHit*>> m_map;
		std::map<TranslationTable::INT_VETO_Index_t,vector<IntVetoDigiHit*>>::iterator m_map_it;
		TranslationTable::INT_VETO_Index_t m_channel;
		int m_isMC;
};

#endif // _IntVetoDigiHit_factory_MC_

