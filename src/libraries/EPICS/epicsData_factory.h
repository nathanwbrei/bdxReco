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
		~epicsData_factory(){};


	private:
		jerror_t init();						///< Called once at program start.
		jerror_t brun(JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun();						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini();						///< Called after last event of last event source has been processed.

		int m_deltaTime; 						//this is the time (in s) added to the each EPICS entry - to account for possible mis-matches.
};

#endif // _epicsData_factory_

