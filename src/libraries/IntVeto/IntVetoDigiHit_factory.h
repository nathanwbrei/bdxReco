// $Id$
//
//    File: IntVetoDigiHit_factory.h
// Created: Wed Jan 20 16:42:38 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoDigiHit_factory_
#define _IntVetoDigiHit_factory_

#include <JANA/JFactoryT.h>
#include "IntVetoDigiHit.h"
#include <TT/TranslationTable.h>

class IntVetoDigiHit_factory:public JFactoryT<IntVetoDigiHit>{
	public:
		IntVetoDigiHit_factory(){};
		~IntVetoDigiHit_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun() {}
		void Finish() {}

		const TranslationTable *m_tt;


};

#endif // _IntVetoDigiHit_factory_

