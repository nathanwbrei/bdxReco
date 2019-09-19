// $Id$
//
//    File: CataniaEvent_factory.h
// Created: Mon Apr 25 23:25:44 CEST 2016
// Creator: celentan (on Linux dhcp-visitor-enr-116-196.slac.stanford.edu 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _CataniaEvent_factory_
#define _CataniaEvent_factory_

#include <JANA/JFactoryT.h>
#include "CataniaEvent.h"

class CataniaEvent_factory:public JFactoryT<CataniaEvent>{
	public:
		CataniaEvent_factory(){};
		~CataniaEvent_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun() {}
		void Finish() {}

	public:

		double m_EC2_cut;					   //cut on the EC2 energy.
		double m_ExtVeto_timeWindows;          //cut on the ExtVeto time - coincidence with the crystal
		double m_IntVeto_timeWindows;          //cut on the IntVeto time - coincidence with the crystal

		int m_isMC;
};

#endif // _CataniaEvent_factory_

