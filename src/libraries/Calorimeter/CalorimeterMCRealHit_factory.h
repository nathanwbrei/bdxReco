// $Id$
//
//    File: CalorimeterMCRealHit_factory.h
// Created: Wed Feb 22 15:46:35 CET 2017
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterMCRealHit_factory_
#define _CalorimeterMCRealHit_factory_

#include <JANA/JFactoryT.h>
#include "CalorimeterMCRealHit.h"

class CalorimeterMCRealHit_factory: public JFactoryT<CalorimeterMCRealHit> {
public:
	CalorimeterMCRealHit_factory() {
	}
	;
	~CalorimeterMCRealHit_factory() {
	}
	;

private:
	void Init() override;
	void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
	void Process(const std::shared_ptr<const JEvent>& aEvent) override;
	void EndRun() {}
	void Finish() {}

	int isMC;

	std::map<TranslationTable::CALO_Index_t, CalorimeterMCRealHit*> m_map;
	std::map<TranslationTable::CALO_Index_t, CalorimeterMCRealHit*>::iterator m_map_it;
};

#endif // _CalorimeterMCRealHit_factory_

