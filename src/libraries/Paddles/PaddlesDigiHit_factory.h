// $Id$
//
//    File: ExtVetoDigiHit_factory.h
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _PaddlesDigiHit_factory_
#define _PaddlesDigiHit_factory_

#include <JANA/JFactoryT.h>
#include <Paddles/PaddlesDigiHit.h>
class TranslationTable;
class PaddlesDigiHit_factory:public JFactoryT<PaddlesDigiHit>{
	public:
		PaddlesDigiHit_factory(){};
		~PaddlesDigiHit_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun() {}
		void Finish() {}

		const TranslationTable *m_tt;
};

#endif // _PaddlesDigiHit_factory_

