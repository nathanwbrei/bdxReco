// $Id$
//
//    File: triggerData_factory.cc
// Created: Fri May  3 11:44:17 CEST 2019
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "triggerDataBDXmini_factory.h"
#include <DAQ/eventData.h>

//------------------
// init
//------------------
jerror_t triggerDataBDXmini_factory::init(void) {
	m_isMC = 0;
	japp->GetParameter("MC", m_isMC);

	/*Min and max time to assume that a channel fired when trigger fired*/
	m_chanTimeMin = -4 * 4;
	m_chanTimeMax = 4 * 4;

	japp->SetDefaultParameter("BDXMINI:TRG_MIN_TIME", m_chanTimeMin, "Trigger time - min ");
	japp->SetDefaultParameter("BDXMINI:TRG_MAX_TIME", m_chanTimeMax, "Trigger time - max");

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t triggerDataBDXmini_factory::brun(JEventLoop *eventLoop, int32_t runnumber) {
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t triggerDataBDXmini_factory::evnt(JEventLoop *loop, uint64_t eventnumber) {
	const eventData* tData;

	int nTriggers = 0;
	int nTriggerSingles[nTriggersMAX] = { 0 };
	vector<int> trgTimes[nTriggersMAX];

	double max = -9999;

	bool triggers[nTriggersMAX] = { false };
	bool channels[nChansMAX] = { false };
	bool channelsTRG[nChansMAX] = { false };

	unsigned int word1, word2, word3, word4;


	long int chanTime;
	unsigned long int chanMask;
	unsigned long int testWord;
	int ii, jj, kk, nwords;

	/*Check - MC do nothing*/
	if (m_isMC) {
		return NOERROR;
	}

	try {
		loop->GetSingle(tData);
	} catch (unsigned long e) {
		jout << " triggerDataBDXmini_factory::evnt no eventData bank this event" << std::endl;
		return OBJECT_NOT_AVAILABLE;
	}
	/*If this is an EPICS event, do nothing.*/
	if (tData->eventType == eventSource::EPICS) {
		return NOERROR;
	}

	for (ii = 0; ii < tData->triggerWords.size() / 2; ii++) {
		word1 = tData->triggerWords[ii * 2];
		word2 = tData->triggerWords[ii * 2 + 1];
		if ((word1 == MAGIC_TRG_WORD) && (word2 == MAGIC_TRG_WORD)) break;

		for (kk = 0; kk < nTriggersMAX; kk++) {
			if ((word1 >> kk) & 0x1) {
				nTriggers++;
				nTriggerSingles[kk]++;
				trgTimes[kk].push_back(word2);
			}
		}
	}
	ii++;
	nwords = tData->triggerWords.size() - ii * 2; //these are the remaining words
	nwords /= 4; //in blocks of 4-words

	/*Second part: single channel words*/
	for (jj = 0; jj < nwords; jj++) {
		word1 = tData->triggerWords[ii * 2 + jj * 4];     //first 32-bits
		word2 = tData->triggerWords[ii * 2 + jj * 4 + 1];   //second 32-bits
		word3 = tData->triggerWords[ii * 2 + jj * 4 + 2];  //TIME, LSB
		word4 = tData->triggerWords[ii * 2 + jj * 4 + 3];  //TIME, MSB

		chanTime = ((unsigned long int)word4 << 32) + ((unsigned long int)word3);
		chanMask = ((unsigned long int)word2 << 32) + ((unsigned long int)word1);


		for (int kk = 0; kk < nChansMAX; kk++) {
			testWord = (unsigned long int)(((unsigned long int)0x1) << kk);
			if ((chanMask & testWord) != 0) {
				channels[kk] = true;
				if ((chanTime >= m_chanTimeMin) && (chanTime <= m_chanTimeMax)) {
					channelsTRG[kk] = true;

				}
			}
		}
	}



	triggerDataBDXmini *m_triggerDataBDXmini = new triggerDataBDXmini();

	m_triggerDataBDXmini->rawWords = tData->triggerWords;

	m_triggerDataBDXmini->setNTriggers(nTriggers);
	for (int ii = 0; ii < nTriggersMAX; ii++) {
		for (int kk = 0; kk < nTriggerSingles[ii]; kk++) {
			m_triggerDataBDXmini->addTriggerTime_single(ii, trgTimes[ii][kk]);
		}
	}
	for (int ii = 0; ii < nChansMAX; ii++) {
		if (channels[ii]) m_triggerDataBDXmini->setChannel(ii);
		if (channelsTRG[ii]){
			m_triggerDataBDXmini->setChannelTRG(ii);
		}
	}

	_data.push_back(m_triggerDataBDXmini);
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t triggerDataBDXmini_factory::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t triggerDataBDXmini_factory::fini(void) {
	return NOERROR;
}

