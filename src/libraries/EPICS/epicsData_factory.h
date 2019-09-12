// $Id$
//
//    File: epicsData_factory.h
// Created: Fri Mar  9 11:22:49 EST 2018
// Creator: celentan (on Darwin celentano-macbook 17.4.0 i386)
//

#ifndef _epicsData_factory_
#define _epicsData_factory_

#include <JANA/JFactory.h>
#include <system/BDXFactory.h>
#include "epicsData.h"

class epicsData_factory:public BDXFactory<epicsData>{
	public:
		epicsData_factory();
		~epicsData_factory() = default;

	private:

        void Init() override;
        void ChangeRun(const std::shared_ptr<const JEvent>&) override;
        void Process(const std::shared_ptr<const JEvent>&) override;

		int m_deltaTime; 						//this is the time (in s) added to the each EPICS entry - to account for possible mis-matches.
};

#endif // _epicsData_factory_

