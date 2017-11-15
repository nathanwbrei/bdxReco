#ifndef _JEVENT_SOURCEEVIODAQ_H_
#define _JEVENT_SOURCEEVIODAQ_H_

// #include <vector>
// #include <string>
// #include <fstream>
// using namespace std;

// JANA headers
#include <JANA/JEventSource.h>
#include <JANA/jerror.h>

using namespace jana;

// bdx headers

// EVIO headers
#include <evioUtil.hxx>
#include <evioFileChannel.hxx>
//EVIO reader header
#include <evioReader/EvioCompositeDecoder.h>

using namespace evio;

// banks header
#ifdef MC_SUPPORT_ENABLE
#include "banks.h"
#endif

//ET
#ifdef ET_SUPPORT_ENABLE
#include "et.h"
#endif

class JEventSourceEvioDAQ: public JEventSource {
public:

	enum DAQSourceType {
		kNoSource, kFileSource, kETSource
	};

	JEventSourceEvioDAQ(const char* source_name);
	virtual ~JEventSourceEvioDAQ();

	virtual const char* className(void) {
		return static_className();
	}
	static const char* static_className(void) {
		return "JEventSourceEvioDAQ";
	}

	jerror_t GetEvent(JEvent &event);
	void FreeEvent(JEvent &event);
	jerror_t GetObjects(JEvent &event, JFactory_base *factory);
	DAQSourceType GetDAQSourceType(void) {
		return source_type;
	}


private:
	evioFileChannel *chan;   // EVIO input channel
	evioDOMTree *EDT;        // single-event evio-DOM-tree
	int vme_mother_tag;    //tag of the bank containing banks with data from VME
	int child_mode1_tag;
	int child_mode7_tag;
	int child_trigger_tag;
	int eventHeader_tag;
	int eventHeader_CODA_tag;
	int prestart_tag;
	int end_tag;
	int curRunNumber, curEventNumber;
	int overwriteRunNumber;

	int m_VERBOSE;
	float TIMEOUT;

	uint32_t BUFFER_SIZE;

	DAQSourceType source_type;

	//ET stuff
	void ConnectToET(const char* source_name);
	bool et_connected;
	int ET_STATION_NEVENTS;
	bool ET_STATION_CREATE_BLOCKING;
	bool quit_on_next_ET_timeout;
	uint32_t *buff;
#ifdef ET_SUPPORT_ENABLE
	et_sys_id sys_id;
	et_att_id att_id;
	et_stat_id sta_id;
#endif

};

#endif
