// $Id$
//
//    File: TEvent_factory_BDXmini.h
// Created: Tue Nov  7 18:43:32 CET 2017
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _TEvent_factory_BDXmini_
#define _TEvent_factory_BDXmini_

#include <JANA/JFactoryT.h>
#include <JANA/Services/JGlobalRootLock.h>
#include "TEvent.h"

class TEvent_factory_BDXmini: public JFactoryT<TEvent> {
public:
	TEvent_factory_BDXmini();
	~TEvent_factory_BDXmini() {}

private:
	void Init() override;
	void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
	void Process(const std::shared_ptr<const JEvent>& aEvent) override;
	void EndRun();
	void Finish();

	TClonesArray *m_CaloHits;
	TClonesArray *m_IntVetoHits;
	TClonesArray *m_fa250Mode1CalibPedSubHits;

#ifdef MC_SUPPORT_ENABLE
	TClonesArray *m_GenParticles;
	TClonesArray *m_CaloMCRealHits;
#endif

	int m_isMC, m_MCRunNumber;
	std::string m_tag;

	int m_ADD_TRIGGER_WORDS;
	int m_thrNpheVeto;
	double m_thrEneTot;
	std::shared_ptr<JGlobalRootLock> m_root_lock;
};

#endif // _TEvent_factory_BDXmini_

