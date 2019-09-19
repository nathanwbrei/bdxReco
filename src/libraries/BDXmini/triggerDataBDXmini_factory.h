// $Id$
//
//    File: triggerData_factory.h
// Created: Fri May  3 11:44:17 CEST 2019
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _triggerData_factory_
#define _triggerData_factory_

#include <JANA/JFactoryT.h>
#include "triggerDataBDXmini.h"

class triggerDataBDXmini_factory:public JFactoryT<triggerDataBDXmini>{
	public:
		triggerDataBDXmini_factory(){};
		~triggerDataBDXmini_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;

		int m_isMC;
		int m_chanTimeMin; //25 ns units;
		int m_chanTimeMax; //25 ns units;

		static const int nTriggersMAX=32;
		static const int nChansMAX=64;
		static const unsigned int MAGIC_TRG_WORD = 0xabcdabcd;
};

#endif // _triggerData_factory_

