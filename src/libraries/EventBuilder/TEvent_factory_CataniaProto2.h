// $Id$
//
//    File: TEvent_factory_CataniaProto2.h
// Created: Thu Dec 22 12:40:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _TEvent_factory_CataniaProto2_
#define _TEvent_factory_CataniaProto2_

#include <JANA/JFactoryT.h>
#include <JANA/Services/JGlobalRootLock.h>
#include "TEvent.h"
#include <string.h>

class TClonesArray;


class TEvent_factory_CataniaProto2:public JFactoryT<TEvent>{
	public:
		TEvent_factory_CataniaProto2() : JFactoryT<TEvent>("TEvent", "CataniaProto2"){};
		~TEvent_factory_CataniaProto2(){};

	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();


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

