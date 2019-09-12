// $Id$
//
//    File: TEvent_factory_CataniaProto2.h
// Created: Thu Dec 22 12:40:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _TEvent_factory_CataniaProto2_
#define _TEvent_factory_CataniaProto2_

#include <JANA/JFactory.h>
#include <JANA/Services/JGlobalRootLock.h>
#include "TEvent.h"
#include <string.h>

class TClonesArray;


class TEvent_factory_CataniaProto2:public jana::JFactory<TEvent>{
	public:
		TEvent_factory_CataniaProto2(){};
		~TEvent_factory_CataniaProto2(){};
		const char* Tag(void){return "CataniaProto2";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.


		TClonesArray *m_CaloDigiHits;
		TClonesArray *m_IntVetoDigiHits;
		TClonesArray *m_ExtVetoDigiHits;

		TClonesArray *m_CaloClusters;
		TClonesArray *m_CaloHits;
		TClonesArray *m_IntVetoHits;
		TClonesArray *m_ExtVetoHits;

		TClonesArray *m_CaloMCRealHits;

		int m_isMC,m_MCRunNumber;
		std::string m_tag;
		std::shared_ptr<JGlobalRootLock> m_root_lock;

};

#endif // _TEvent_factory_CataniaProto2_

