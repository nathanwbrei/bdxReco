#ifndef _EventProcessor_
#define _EventProcessor_

// JANA headers
#include <JANA/JEventProcessor.h>
#include <JANA/JApplication.h>
#include <JANA/Services/JGlobalRootLock.h>
#include <system/CalibrationHandler.h>

#include <vector>
#include <utility>
#include <map>
#include <algorithm>

using namespace std;
// EVIO headers
//#include "evioUtil.hxx"
//#include "evioFileChannel.hxx"
//using namespace evio;

//ROOT headers
#include "TFile.h"

class TranslationTable;
class JOutput;
class TTree;
class CataniaEventProto2;
class TEvent;

/*This class is the "main" event processor, that gets called in any case,
 * also if we use users plugins
 *
 * Here I want to handle the output in a proper way.
 */

class JEventLoop; // TODO: Temporary so that everything compiles again -- N.B.

class BDXEventProcessor:public JEventProcessor
{
public:
	BDXEventProcessor();
	~BDXEventProcessor();
	const char* className(void){return "BDXEventProcessor";}  /*A.C. DO NOT TOUCH THIS!*/

	JOutput* getOutput(){return m_output;}

	void addCalibration(CalibrationHandlerBase* cal);
	void updateCalibration(CalibrationHandlerBase* cal, JEventLoop* eventLoop);
	void clearCalibration(CalibrationHandlerBase* cal);
private:
	jerror_t init();                                 // Called once at program start.
	jerror_t brun(JEventLoop*, int32_t runnumber);       // Called everytime a new run number is detected.
	jerror_t evnt(JEventLoop*, uint64_t eventnumber);     // Called every event.
	jerror_t erun();                                 // Called everytime run number changes, provided brun has been called.
	jerror_t fini();                                 // Called after last event of last event source has been processed.

	/*Following part is very critical. We can't have CalibrationHandlers in each factory,
	 * since this requires access to calibrations in each thread. And this may be time consuming,
	 * specially if using remote mysql.
	 * Therefore,
	 * - Have the factories creating their own calibrations
	 * - Have them adding these to the m_calibrations vector, containing a pair of the pointer and a string to the CCDB path
	 * - Have factories getting back the pointer in their brun method
	 */
	map<string,vector<CalibrationHandlerBase*> >  m_calibrations;
	map<string,vector<CalibrationHandlerBase*> >::iterator m_calibrations_it;


	string           optf;     // Output file
	string			 outType,outFile;
	int isET;

	JOutput		*m_output;
	const TranslationTable *m_tt;
	int m_isMC;


	/*The EventHeader tree*/
	TTree *m_eventHeader;
	int eventN,runN,eventT;


	/*Time*/
	Long64_t startTime,stopTime,deltaTime;

	/*The RunInfo tree*/
	TTree *m_runInfo;

	/*Do we want to build the DST?*/
	string m_buildDST;
	int m_DObuildDST;

	TTree *m_eventDST;
	const TEvent* m_event;

	int isFirstCallToBrun;

	JStreamLog bout;
	JStreamLog berr;
	std::shared_ptr<JGlobalRootLock> m_root_lock;
};

#endif

