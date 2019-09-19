// $Id$
//
//    File: ExtVetoDigiHit_factory.h
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoDigiHit_factory_
#define _ExtVetoDigiHit_factory_

#include <JANA/JFactoryT.h>
#include "ExtVetoDigiHit.h"
class TranslationTable;
class ExtVetoDigiHit_factory:public JFactoryT<ExtVetoDigiHit>{
public:
	ExtVetoDigiHit_factory(){};
	~ExtVetoDigiHit_factory(){};


private:
	void Init() override;
	void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
	void Process(const std::shared_ptr<const JEvent>& aEvent) override;

	const TranslationTable *m_tt;


};

#endif // _ExtVetoDigiHit_factory_

