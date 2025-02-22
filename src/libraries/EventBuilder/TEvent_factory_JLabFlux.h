// $Id$
//
//    File: TEvent_factory_JLabFlux.h
// Created: Tue Nov  7 18:43:32 CET 2017
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _TEvent_factory_JLabFlux_
#define _TEvent_factory_JLabFlux_

#include <JANA/JFactoryT.h>
#include <JANA/Services/JGlobalRootLock.h>
#include "TEvent.h"

class TEvent_factory_JLabFlux: public JFactoryT<TEvent> {
public:
	TEvent_factory_JLabFlux(): JFactoryT<TEvent>("TEvent", "JLabFlux") {}
	~TEvent_factory_JLabFlux() {}

private:
	void Init() override;
	void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
	void Process(const std::shared_ptr<const JEvent>& aEvent) override;
	void EndRun();
	void Finish();

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

