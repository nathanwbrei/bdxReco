// $Id$
//
//    File: epicsData_factory.cc
// Created: Fri Mar  9 11:22:49 EST 2018
// Creator: celentan (on Darwin celentano-macbook 17.4.0 i386)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "epicsData_factory.h"
#include <DAQ/epicsRawData.h>
#include <DAQ/eventData.h>
using namespace jana;

/*This is the global memory to keep track of epics data*/
epicsData m_data;

epicsData_factory::epicsData_factory(){

	m_deltaTime=0;
	gPARMS->SetDefaultParameter("EPICS:DELTA_TIME",m_deltaTime,"Time (in s) to add to all EPICS times - can be >0, =0(default), <0");
}
//------------------
// init
//------------------
jerror_t epicsData_factory::init(void) {
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t epicsData_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber) {
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t epicsData_factory::evnt(JEventLoop *loop, uint64_t eventnumber) {

	vector<const epicsRawData*> m_rawdata;
	const eventData* tData;
	loop->Get(m_rawdata);

	try {
		loop->GetSingle(tData);
	} catch (unsigned long e) {
		jout << "epicsData_factory::evnt no eventData bank this event" << endl;
		return OBJECT_NOT_AVAILABLE;
	}

	/*If m_rawdata.size is greater than 0, it means that the event is an epics event,
	 * and that the JEventSourceEVIODAQ got the epics data from EVIO/ET.
	 * By definition, this event is a sequential event, one thread only will process this.
	 * We can save everything to global variables. This is thread-safe, only one thread is doing so
	 */

	/*If no epics raw data was found, then this is a regular event. Copy the epics data from the
	 * last epics event, by reading from the global memory. This is thread-save, we just read from it
	 */
	if (m_rawdata.size() > 0) {
		m_data.runNumber = tData->runN;
		m_data.time = tData->time+m_deltaTime;
		for (int ii = 0; ii < m_rawdata.size(); ii++) {
			jout << eventnumber << ": Extracting EPICS data: " << m_rawdata[ii]->rawData << std::endl;
			m_data.decode(m_rawdata[ii]->rawData,m_deltaTime);
		}
	}

	epicsData *data = new epicsData();
	*data = m_data;

	_data.push_back(data);
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t epicsData_factory::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t epicsData_factory::fini(void) {
	return NOERROR;
}

