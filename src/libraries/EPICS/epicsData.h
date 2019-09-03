// $Id$
//
//    File: epicsData.h
// Created: Fri Mar  9 11:22:49 EST 2018
// Creator: celentan (on Darwin celentano-macbook 17.4.0 i386)
//

#ifndef _epicsData_
#define _epicsData_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "TObject.h"

#include <map>
using namespace std;

class epicsData: public JObject, public TObject {
private:

	map<string, double> values;
	map<string, int> times;
public:
	JOBJECT_PUBLIC(epicsData);

	void Summarize(JObjectSummary& summary) const final {}

	int runNumber, eventNumber, time;
	bool hasData(string key) const;
	double getDataValue(string key) const;
	int getDataTime(string key) const;


	void decode(string rawData,int deltaTime);


	epicsData() {};
	virtual ~epicsData() {};

	/*TObject virtual methods*/
	virtual void Print(Option_t *option="") const;

	ClassDef(epicsData,1);
};

#endif // _epicsData_

