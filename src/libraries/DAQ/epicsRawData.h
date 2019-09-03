#ifndef _epicsRawData_
#define _epicsRawData_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include "TObject.h"
#include "TString.h"

/*This is basically a light-weight container for EPICS data from EVIO*/
class epicsRawData : public JObject, public TObject {

	public:
		JOBJECT_PUBLIC(epicsRawData);

		string rawData;

		void Summarize(JObjectSummary& summary) const final {}
};

#endif

