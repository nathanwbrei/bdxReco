// $Id$
//
//    File: triggerData.h
// Created: Thu Jan 28 18:19:51 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _eventData_
#define _eventData_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include "TObject.h"
#include "TString.h"

typedef enum {
	DAQ=1,EPICS=2,CONF=3
} eventSource;

class eventData:public JObject, public TObject {
	public:
		JOBJECT_PUBLIC(eventData);
		
		unsigned long int eventN;
		unsigned long int eventTS; //This has been added for the TRIDAS system, where an event is identified uniquely by an TS id and an event ID
		unsigned int runN;
		unsigned long int time; //event time in UNIX time stamp (seconds)
		std::vector<uint32_t> triggerWords;
		
		int eventType;


		void Summarize(JObjectSummary& summary) const final {
			summary.add(runN, "runN", "%i");
			summary.add(eventN, "eventN", "%lu");
			summary.add(eventTS, "eventTS", "%lu");
			summary.add(time, "time", "%lu");
			for (int iword=0;iword<triggerWords.size();iword++){
			    summary.add(triggerWords[iword], Form("word_%i", iword), "%x");
			}
		}
		
		eventData(){
			eventType=0;
			eventN=0;
			eventTS=0;
			runN=0;
			time=0;
		}

};

#endif // _eventData_

