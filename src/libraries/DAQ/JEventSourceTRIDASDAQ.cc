#include "JEventSourceTRIDASDAQ.h"

// C++ headers
#include <iostream>
#include <time.h>
#include <unistd.h>
using namespace std;

// JANA headers
#include <JANA/JEvent.h>
#include <JANA/JApplication.h>
#include <JANA/JFactoryT.h>

//DAQ
#include <DAQ/fa250WaveboardV1Hit.h>
#include <DAQ/eventData.h>
#include <DAQ/epicsRawData.h>

// Constructor
JEventSourceTRIDASDAQ::JEventSourceTRIDASDAQ(const char* source_name, JApplication* app) :
		JEventSource(source_name, app), curRunNumber(0), curEventNumber(0), ptReader(0), ptTimeSlice(0), ptEvent(0) {

	jout << "JEventSourceTRIDASDAQ creator: " << this << jendl;

	curEventType = 0;
	source_type = kNoSource;
	overwriteRunNumber = -1;

	//TODO: check if file exists
	ptReader = new PtFileReader<sample::uncompressed>(source_name);
	source_type = kFileSource;

	//point to the first time slice
	it_ptReader = ptReader->begin();
	nEventsTimeSlice = (*it_ptReader).nEvents();
	//Create a new time slice
	ptTimeSlice = new TimeSlice<sample::uncompressed>(*it_ptReader);
	it_ptTimeSlice = ptTimeSlice->begin();

	//currEventTimeSlice = 0;
	jout << "JEventSourceTRIDASDAQ creator DONE: " << this << jendl;

}

// Destructor
JEventSourceTRIDASDAQ::~JEventSourceTRIDASDAQ() {
	cout << " Closing input file " << GetResourceName() << " ." << endl;

}

// GetEvent
void JEventSourceTRIDASDAQ::GetEvent(std::shared_ptr<JEvent> event) {

	if (this->source_type == kFileSource) {
		//The counter on events is at the end of current time slice. Try to move to next time slice
		//if (currEventTimeSlice == nEventsTimeSlice) {
		if (it_ptTimeSlice == ptTimeSlice->end()) {
			it_ptReader++;
			delete ptTimeSlice;
			//It the iterator on time slices is at the end, the source is completely read.
			if (it_ptReader == ptReader->end()) {
				jout << "Source done" << jendl;
				fflush(stdout);
				throw RETURN_STATUS::kNO_MORE_EVENTS;
			} else {
				nEventsTimeSlice = (*it_ptReader).nEvents();
				ptTimeSlice = new TimeSlice<sample::uncompressed>(*it_ptReader);
				it_ptTimeSlice = ptTimeSlice->begin();
				//currEventTimeSlice = 0;
			}
		}

		ptEvent = new Event<sample::uncompressed>(*it_ptTimeSlice);

		event->Insert(ptEvent); //event->SetRef((void*) ptEvent);
		event->Insert(ptTimeSlice); // event->SetEventTS(ptTimeSlice->id());
		event->SetJEventSource(this);
		event->SetEventNumber(ptEvent->id());

		curRunNumber = ptReader->runNumber();
		fflush(stdout);
		if (overwriteRunNumber != -1) {
			event->SetRunNumber(overwriteRunNumber);
		} else {
			event->SetRunNumber(curRunNumber);
		}
		it_ptTimeSlice++;
		//	currEventTimeSlice++;
		return;

	} else {
		throw RETURN_STATUS::kERROR;
		// EVENT_SOURCE_NOT_OPEN;
	}
}

// GetObjects
bool JEventSourceTRIDASDAQ::GetObjects(const std::shared_ptr<const JEvent>& event, JFactory* factory) {
/// This gets called through the virtual method of the
/// JEventSource base class. It creates the objects of the type
/// which factory is based.

/// Example: DCsegment needs DCHit. If DCHit doesn't exist already, then
/// it will be read here.
    // We must have a factory to hold the data
	if (!factory) return false;

    //As suggested by David, do a check on the factory type to decide what to do
	JFactoryT<fa250WaveboardV1Hit> *fac_fa250WaveboardV1Hit = dynamic_cast<JFactoryT<fa250WaveboardV1Hit>*>(factory);
	JFactoryT<eventData> *fac_eventData = dynamic_cast<JFactoryT<eventData>*>(factory);

	if (fac_eventData != NULL) {
		vector<eventData*> data;
		eventData *this_eventData = new eventData();
		this_eventData->eventType = DAQ;

		Event<sample::uncompressed> *ptEvent_pointer = (Event<sample::uncompressed>*) event->GetRef();

		//take the first hit time (4 ns from 1 Jan. 2000) -- all hits in the event are within the same second!
		fine_time minTime = getDFHFullTime((*(ptEvent_pointer->begin())).frameHeader(0));

		this_eventData->time = std::chrono::duration_cast<std::chrono::seconds>(minTime).count(); //seconds from 1 Jan. 2000
		this_eventData->time += 946684800; //unix time of 1 Jan 2000 first second

		this_eventData->runN = event->GetRunNumber();
		this_eventData->eventN = event->GetEventNumber();
		this_eventData->eventTS = event->GetEventTS();

		this_eventData->triggerWords.push_back(Event<sample::uncompressed>::max_triggers_number); //currently 5
		for (int ii = 0; ii < Event<sample::uncompressed>::max_triggers_number; ii++) {
			this_eventData->triggerWords.push_back(ptEvent_pointer->nseeds(ii));
		}

		this_eventData->triggerWords.push_back(Event<sample::uncompressed>::max_plugins_number); //currently 8
		for (int ii = 0; ii < Event<sample::uncompressed>::max_plugins_number; ii++) {
			this_eventData->triggerWords.push_back(ptEvent_pointer->plugin_nseeds(ii));
		}
		for (int ii = 0; ii < Event<sample::uncompressed>::max_plugins_number; ii++) {
			this_eventData->triggerWords.push_back(ptEvent_pointer->plugin_trigtype(ii));
		}

		data.push_back(this_eventData);
		fac_eventData->Set(data);
		return true;
	}
	if (fac_fa250WaveboardV1Hit != NULL) {
		vector<fa250WaveboardV1Hit*> data;
		Event<sample::uncompressed> *ptEvent_pointer = (Event<sample::uncompressed>*) event->GetRef();
		fine_time minTime = getDFHFullTime((*(ptEvent_pointer->begin())).frameHeader(0));  //take the first hit time

		//First, find the min. hit time
		for (Event<sample::uncompressed>::iterator it = ptEvent_pointer->begin(); it != ptEvent_pointer->end(); ++it) {

			Hit<sample::uncompressed> hit = (*it);	//This is the HIT
			auto abs_time = getDFHFullTime(hit.frameHeader(0));  //absolute time in 4 ns
			if (abs_time < minTime) minTime = abs_time;
		}

		//Then get hits
		for (Event<sample::uncompressed>::iterator it = ptEvent_pointer->begin(); it != ptEvent_pointer->end(); ++it) {
			Hit<sample::uncompressed> hit = (*it);	//This is the HIT
			fa250WaveboardV1Hit *fahit = new fa250WaveboardV1Hit();

			auto abs_time = getDFHFullTime(hit.frameHeader(0));  //absolute time in 4 ns
			auto delta_time = abs_time - minTime; //always >= 0a
			auto Ndelay = delta_time.count();

			fahit->m_channel.rocid = hit.frameHeader(0).TowerID;
			fahit->m_channel.slot = hit.frameHeader(0).EFCMID;
			fahit->m_channel.channel = hit.frameHeader(0).PMTID;

			fahit->chargeFirstDBnopedsub = hit.frameHeader(0).Charge;

			fahit->trigger = 0; //TODO
			fahit->timestamp = abs_time.count();

			/*To account for delays between waveforms,
			 * a number of fake samples is added at the beginning of each, equivalent to the relative delay with respect to the earliest waveform.
			 * The fake sample is the first sample in the waveform
			 */
			auto firstSample = *(hit.begin());
			for (int ii = 0; ii < Ndelay; ii++) {
				fahit->samples.push_back(firstSample);
			}

			for (auto it_ptSample = hit.begin(); it_ptSample != hit.end(); it_ptSample++) {
				fahit->samples.push_back(*it_ptSample);
			}

			data.push_back(fahit);
		}

		fac_fa250WaveboardV1Hit->Set(data);

	}
	return false;
}

