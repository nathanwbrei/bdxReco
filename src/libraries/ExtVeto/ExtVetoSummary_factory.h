// $Id$
//
//    File: ExtVetoSummary_factory.h
// Created: Fri Mar 11 16:16:37 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoSummary_factory_
#define _ExtVetoSummary_factory_

#include <JANA/JFactoryT.h>
#include "ExtVetoSummary.h"
#include <system/CalibrationHandler.h>

class ExtVetoSummary_factory:public JFactoryT<ExtVetoSummary>{
	public:
		ExtVetoSummary_factory(){};
		~ExtVetoSummary_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();

		std::map<int,ExtVetoSummary*> m_map;
		std::map<int,ExtVetoSummary*>::iterator m_it;
};

#endif // _ExtVetoSummary_factory_

