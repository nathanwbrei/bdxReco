// $Id$
//
//    File: CalorimeterDigiHit_factory_MC.h
// Created: Tue Mar  1 17:38:32 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _CalorimeterDigiHit_factory_MC_
#define _CalorimeterDigiHit_factory_MC_

#include <JANA/JFactoryT.h>
#include "CalorimeterDigiHit.h"

class CalorimeterMCHit;

class CalorimeterDigiHit_factory_MC:public JFactoryT<CalorimeterDigiHit>{
public:
	CalorimeterDigiHit_factory_MC() : JFactoryT<CalorimeterDigiHit>("CalorimeterDigiHit", "MC"){};
	~CalorimeterDigiHit_factory_MC(){};

	static void SetIndex(TranslationTable::CALO_Index_t &index,const CalorimeterMCHit *mchit,int MC);



private:
	void Init() override;
	void ChangeRun(const std::shared_ptr<const JEvent>& event) override {}
	void Process(const std::shared_ptr<const JEvent>& aEvent) override;
	void EndRun() {}
	void Finish() {}

	/*This code is here because MC could generate more than 1 hit per sector!*/
	/*The key is the way MC is organized: sector - channel (where sector and channel have a different meaning than in the real data!!!)*/
	std::map<TranslationTable::CALO_Index_t,CalorimeterDigiHit*> m_map;
	std::map<TranslationTable::CALO_Index_t,CalorimeterDigiHit*>::iterator m_map_it;

	int m_isMC;
};

#endif // _CalorimeterDigiHit_factory_MC_

