// $Id$
//
//    File: ExtVetoHit_factory.h
// Created: Wed Jan 13 21:06:44 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _PaddlesHit_factory_
#define _PaddlesHit_factory_

#include <JANA/JFactoryT.h>
#include <Paddles/PaddlesHit.h>
#include <system/CalibrationHandler.h>

class PaddlesHit_factory:public JFactoryT<PaddlesHit>{
	public:
		PaddlesHit_factory():m_ENE_gain(0),m_tt(0){isMC=0;};
		~PaddlesHit_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();

		const TranslationTable *m_tt;
		int isMC;
		CalibrationHandler<TranslationTable::PADDLES_Index_t> *m_ENE_gain;

};

#endif // _PaddlesHit_factory_

