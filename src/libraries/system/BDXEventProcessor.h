#ifndef _EventProcessor_
#define _EventProcessor_

// JANA headers
#include <JANA/JApplication.h>
#include <JANA/JEventProcessor.h>
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
class TEvent;

/* This class is the "main" event processor, that gets called in any case, also if we use users plugins
 * Here I want to handle the output in a proper way.
 */

class BDXEventProcessor:public JEventProcessor
{
public:
	BDXEventProcessor();
	~BDXEventProcessor();
	const char* className(void){return "BDXEventProcessor";}  /*A.C. DO NOT TOUCH THIS!*/

	JOutput* getOutput(){return m_output;}

private:
    void Init() override;
	void Process(const std::shared_ptr<const JEvent>& aEvent) override;
	void Finish() override;

	// JANA2 does not support callbacks for begin/end run, so we hack around this for now
	void BeginRun(const std::shared_ptr<const JEvent>& event);
	void EndRun();
	uint64_t m_last_run_number = std::numeric_limits<uint64_t>::max();


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

	JLogger bout;
	JLogger berr;
	std::shared_ptr<JGlobalRootLock> m_root_lock;
};

#endif

