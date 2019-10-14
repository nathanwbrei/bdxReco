// C++ headers
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
using namespace std;

// bdx headers
#include "BDXEventProcessor.h"
#include "string_utilities.h"
//DAQ
#include <DAQ/eventData.h>
//EPICS
#include <EPICS/epicsData.h>
// TT
#include <TT/TranslationTable.h>

//DST
#include <EventBuilder/TEvent.h>
#include <EventBuilder/TEventHeader.h>

#include "TTree.h"

#include "JOutput.h"
#include "JROOTOutput.h"

// Constructor
BDXEventProcessor::BDXEventProcessor() :
		m_output(0), m_tt(0), m_isMC(0), m_eventDST(0), m_runInfo(0), m_event(0), eventN(0), m_eventHeader(0), eventT(0), deltaTime(0), runN(0), isET(0) {

	bout.SetTag("BDXEventProcessor >>");
	berr.SetTag("BDXEventProcessorError >>");
	LOG_DEBUG(bout) << "BDXEventProcessor creator start" << jendl;
	optf = "";
	m_DObuildDST = 0;
	m_buildDST = "";
	isFirstCallToBrun = 1;
	startTime = 9999999999;
	stopTime = -9999999999;


	m_root_lock = japp->GetService<JGlobalRootLock>();

	LOG_DEBUG(bout) << "BDXEventProcessor creator end" << jendl;
}

// Destructor
BDXEventProcessor::~BDXEventProcessor() {
	LOG_DEBUG(bout) << "Destroying BDXEventProcessor!" << LOG_END;
}

void BDXEventProcessor::Init() {

	LOG_DEBUG(bout) << "BDXEventProcessor::init" << jendl;
	japp->GetParameter("MC", m_isMC);
	japp->SetDefaultParameter("SYSTEM:BUILD_DST", m_buildDST, "Enable DST, using the form \"TYPE\". TYPE is the name of an existing event builder. Example: -PSYSTEM:BUILD_DST=\"CataniaProto2\" (also: FullMC, JLabFlux)");
	LOG_DEBUG(bout) << "Building DST is: " << m_buildDST << jendl;

	if (m_buildDST.size() == 0) {
		LOG_DEBUG(bout) << "No DST will be built" << jendl;
		m_DObuildDST = 0;
	} else {
		m_DObuildDST = 1;
	}

	japp->SetDefaultParameter("SYSTEM:OUTPUT", optf, "Set OUTPUT file type and name, using the form \"TYPE,FILENAME\". Type can be ROOT, EVIO, TXT. Example: -PSYSTEM:OUTPUT=\"ROOT,out.root\" ");
	LOG_DEBUG(bout) << "Out string is: " << optf << jendl;
	outType.assign(optf, 0, optf.find(","));
	outFile.assign(optf, optf.find(",") + 1, optf.size());

	std::transform(outType.begin(), outType.end(), outType.begin(), ::tolower);

	if (optf != "none") {
		LOG_INFO(bout) << "Again, out string is: " << optf << jendl;
		LOG_INFO(bout) << "Out file type is: " << outType << jendl;
		if (outType == "root") {
			m_output = new JROOTOutput();
		} else if (outType == "root_et") { /*Special case, when we connect to ET-ring*/
#ifdef ET_SUPPORT_ENABLE
			isET = 1;
			m_output = new JROOTOutput();
#else
			LOG_DEBUG(bout) << "root_et file type was requested, but no ET-support was built. Exiting!" << jendl;
			exit(1);
#endif
		} else if (outType == "evio") {
			LOG_ERROR(berr) << "evio not yet implemented" << jendl;
		} else if (outType == "txt") {
			LOG_ERROR(berr) << "txt not yet implemented" << jendl;
		} else {
			LOG_ERROR(berr) << "file type not recognized: " << outType << jendl;
		}

	}

	// lock all root operations
	m_root_lock->acquire_write_lock();

	/*Event header and runInfo are always created - as memory resident TTrees (these are quite small)*/
	gDirectory->cd();

	m_eventHeader = new TTree("EventHeader", "EventHeader");
	m_eventHeader->Branch("eventN", &eventN);
	m_eventHeader->Branch("runN", &runN);
	m_eventHeader->Branch("T", &eventT);


	m_runInfo = new TTree("RunInfo", "RunInfo");
	m_runInfo->Branch("runN", &runN);
	m_runInfo->Branch("dT", &deltaTime);

	m_root_lock->release_lock();
}


void BDXEventProcessor::BeginRun(const std::shared_ptr<const JEvent>& event) {

	auto runnumber = event->GetRunNumber();
	LOG_INFO(bout) << "BDXEventProcessor::brun " << runnumber << "(isFirstCallToBrun: " << isFirstCallToBrun << " m_output: " << m_output << ")" << jendl;

	// lock all root operations
	m_root_lock->acquire_write_lock();

	if (isFirstCallToBrun) {
		if (m_output != 0) {
			LOG_INFO(bout) << "got m_output, className is: " << m_output->className() << jendl;
            JROOTOutput* m_ROOTOutput = (JROOTOutput*) m_output;

            if (isET == 1) {
                outFile = string(Form("out.%i.online.root", runnumber));
                LOG_INFO(bout) << "Running on ET with root thus changing file name to: " << outFile << jendl;
            }
            m_ROOTOutput->OpenOutput(outFile);

            /*DST TTree created as file-resident TTree, only if output exists*/
            if (m_DObuildDST) {
                m_eventDST = new TTree("EventDST", "EventDST");
                m_eventDST->Branch("Event", &m_event);
                if (m_isMC == 0) m_eventDST->AddFriend(m_eventHeader);
                m_ROOTOutput->AddObject(m_eventDST);
            }

            /*If an output exists, add the eventHeader and runInfo*/
            (m_ROOTOutput)->AddObject(m_eventHeader);
            (m_ROOTOutput)->AddObject(m_runInfo);
		}
	}

	if (m_isMC == 0) {
		event->Get(&m_tt);
	}
	// lock all root operations
	m_root_lock->release_lock();
}


void BDXEventProcessor::Process(const std::shared_ptr<const JEvent>& event) {

	const eventData* tData;
	vector<const TEvent*> events;
	const epicsData* eData;

	if (event->GetRunNumber() != m_last_run_number) {
		// We have to handle run begin/end manually because JANA2 no longer takes care of this for us

		if (m_last_run_number != std::numeric_limits<uint64_t>::max()) {
		    // If we already had a valid run number, call EndRun(). Otherwise, don't.
			EndRun();
		}
		m_last_run_number = event->GetRunNumber();
		BeginRun(event);
	}

	/*For non-MC case, check that:
	 * There's a valid eventData (true for both DAQ and EPICS)
	 * There's a valid epicsData (true for both DAQ and EPICS - DAQ uses persistency)
	 * The event is from DAQ - otherwise skip. Don't want to save non-DAQ events.
	 */
	if (m_isMC == 0) {
		try {
			event->Get(&tData);
		} catch (JException e) {
			LOG_INFO(bout) << "No eventData bank this event " << jendl;
			return;
		}
		/*This is the EPICS part. The call here will force getting data from the epicsDataProcessed_factory, that takes care of persistency*/
		try {
			event->Get(&eData);
		} catch (JException e) {
			return;
		}

		if (tData->eventType != eventSource::DAQ) {
		    return;
		}
	}
	if (m_DObuildDST) {
		event->Get(events, m_buildDST);
		if (events.size() != 1) {
			return; // RESOURCE_UNAVAILABLE;
		}
		/*Add EPICS data in case of non-MC*/
		if (m_output != 0) {

			if (m_isMC == 0) events[0]->getEventHeader()->copyEpicsData(eData);

			m_root_lock->acquire_write_lock();
			m_event = events[0];
			m_eventDST->Fill();
			m_root_lock->release_lock();
		}
	}

	if (m_isMC == 0) {
		if (m_output != 0) {
		    m_root_lock->acquire_write_lock();
			eventT = tData->time;
			eventN = event->GetEventNumber();

			runN = tData->runN;
			LOG_DEBUG(bout) << "BDXEventProcessor: Filling ROOT tree " << event->GetEventNumber() << LOG_END;
			m_eventHeader->Fill();
			//Time
			if (eventT < startTime) startTime = eventT;
			if (eventT > stopTime) stopTime = eventT;
			m_root_lock->release_lock();
		}
	}
}


void BDXEventProcessor::EndRun() {

    m_root_lock->acquire_write_lock();
	deltaTime = stopTime - startTime;
	LOG_INFO(bout) << "BDXEventProcessor::erun " << jendl;
	LOG_INFO(bout) << "Run start: " << startTime << " stop: " << stopTime << " diff: " << deltaTime << jendl;
	m_runInfo->Fill();

	LOG_INFO(bout) << "Persisting ROOT file" << LOG_END;
	if (m_output && (isET == 1)) {
		m_output->CloseOutput();
	}
	m_root_lock->release_lock();
}


void BDXEventProcessor::Finish() {

	// We have to handle run begin/end manually because JANA2 no longer takes care of this for us
	DoInitialize(); // In case JANA terminates immediately, makes sure that Init got called
	EndRun();

	// If another EventProcessor is in the list ahead of this one, then
	// it will have finished before this is called. e.g. closed the
	// ROOT file!
	LOG_INFO(bout) << "BDXEventProcessor::Finish called" << jendl;
	fflush(stdout);
	m_root_lock->acquire_write_lock();
	if (m_output) {
		m_output->CloseOutput(); /*This is ok, CloseOutput takes care of m_output already closed*/
	}
	m_root_lock->release_lock();
	LOG_DEBUG(bout) << "BDXEventProcessor Finish ends" << jendl;
	fflush(stdout);
}

