// $Id$
//
//    File: TEvent_factory_JLabFlux.h
// Created: Tue Nov  7 18:43:32 CET 2017
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _TEvent_factory_JLabFlux_
#define _TEvent_factory_JLabFlux_

#include <JANA/JFactory.h>
#include <JANA/Services/JGlobalRootLock.h>
#include "TEvent.h"

class TEvent_factory_JLabFlux: public jana::JFactory<TEvent> {
public:
	TEvent_factory_JLabFlux() {
	}
	;
	~TEvent_factory_JLabFlux() {
	}
	;
	const char* Tag(void) {
		return "JLabFlux";
	}

private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.

	TClonesArray *m_CaloHits;
	TClonesArray *m_IntVetoHits;

#ifdef MC_SUPPORT_ENABLE
	TClonesArray *m_GenParticles;
	TClonesArray *m_CaloMCRealHits;
#endif

	int m_isMC, m_MCRunNumber;
	std::string m_tag;
	std::shared_ptr<JGlobalRootLock> m_root_lock;
};

#endif // _TEvent_factory_JLabFlux_

