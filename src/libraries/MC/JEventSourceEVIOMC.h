#ifndef _JEVENT_SOURCEEVIO_H_
#define _JEVENT_SOURCEEVIO_H_

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
#include "banks.h"


class JEventSourceEvioMC:public JEventSource
{
	public:
		JEventSourceEvioMC(const char* source_name, JApplication* app);
		~JEventSourceEvioMC() override;

		virtual const char* className(void)      {return static_className();}
		static const char* static_className(void){return "JEventSourceEvioMC";}

		jerror_t GetEvent(JEvent &event);
		void FreeEvent(JEvent &event);
		jerror_t GetObjects(JEvent &event, JFactory_base *factory);
	

		map<string, gBank> banksMap;

		typedef struct MCEVIOreference{
			evioDOMTree *EDT;
			Mevent		*event;
		};
	
	private:
		evioFileChannel *chan;   // EVIO input channel
		evioDOMTree *EDT;        // single-event evio-DOM-tree
		Mevent		*evt;

		int curRunNumber,curEventNumber;
		int overwriteRunNumber;
		int MC;
};

#endif
