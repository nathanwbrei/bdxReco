// $Id$
//
//    File: TEvent_factory.h
// Created: Thu Dec 22 12:32:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _TEvent_factory_
#define _TEvent_factory_

#include <JANA/JFactoryT.h>
#include "TEvent.h"

class TEvent_factory:public JFactoryT<TEvent>{
	public:
		TEvent_factory(){};
		~TEvent_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();
};

#endif // _TEvent_factory_

