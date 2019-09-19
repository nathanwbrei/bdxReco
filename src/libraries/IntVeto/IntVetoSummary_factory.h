// $Id$
//
//    File: IntVetoSummary_factory.h
// Created: Tue Mar 29 15:07:39 CEST 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoSummary_factory_
#define _IntVetoSummary_factory_

#include <JANA/JFactoryT.h>
#include "IntVetoSummary.h"

class IntVetoSummary_factory:public JFactoryT<IntVetoSummary>{
	public:
		IntVetoSummary_factory(){};
		~IntVetoSummary_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun() {}
		void Finish() {}

		std::map<int,IntVetoSummary*> m_map;
		std::map<int,IntVetoSummary*>::iterator m_it;



};

#endif // _IntVetoSummary_factory_

