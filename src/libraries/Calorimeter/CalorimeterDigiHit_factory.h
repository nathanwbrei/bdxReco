// $Id$
//
//    File: CalorimeterDigiHit_factory.h
// Created: Wed Jan 27 16:39:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterDigiHit_factory_
#define _CalorimeterDigiHit_factory_

#include <JANA/JFactoryT.h>
#include "CalorimeterDigiHit.h"

class TranslationTable;

class CalorimeterDigiHit_factory:public JFactoryT<CalorimeterDigiHit>{
public:
	CalorimeterDigiHit_factory(){};
	~CalorimeterDigiHit_factory(){};


private:
	void Init() override;
	void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
	void Process(const std::shared_ptr<const JEvent>& aEvent) override;


	int VERBOSE;
};

#endif // _CalorimeterDigiHit_factory_

