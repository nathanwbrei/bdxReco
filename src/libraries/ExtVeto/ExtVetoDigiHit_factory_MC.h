// $Id$
//
//    File: ExtVetoDigiHit_factory_MC.h
// Created: Tue Mar  8 12:19:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoDigiHit_factory_MC_
#define _ExtVetoDigiHit_factory_MC_

#include <JANA/JFactoryT.h>

#include <map>
#include <utility>

class ExtVetoDigiHit;

class ExtVetoDigiHit_factory_MC:public JFactoryT<ExtVetoDigiHit>{
public:
	ExtVetoDigiHit_factory_MC() : JFactoryT<ExtVetoDigiHit>("ExtVetoDigiHit", "MC"){};
	~ExtVetoDigiHit_factory_MC(){};

	int getCataniaV1Component(int MCchannel);
	int getCataniaV2Component(int MCchannel);
	int getFullV1Component(int MCchannel);
	int getFullV2Component(int MCchannel);


private:
	void Init() override;
	void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
	void Process(const std::shared_ptr<const JEvent>& aEvent) override;
	void EndRun();
	void Finish();

	/*This code is here because MC could generate more than 1 hit per sector!*/
	std::map<TranslationTable::EXT_VETO_Index_t,vector<ExtVetoDigiHit*>> m_map;
	std::map<TranslationTable::EXT_VETO_Index_t,vector<ExtVetoDigiHit*>>::iterator m_map_it;
	TranslationTable::EXT_VETO_Index_t m_channel;
	int m_isMC;
};

#endif // _ExtVetoDigiHit_factory_MC_

