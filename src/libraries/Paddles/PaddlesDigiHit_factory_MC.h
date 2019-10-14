// $Id$
//
//    File: PaddlesDigiHit_factory_MC.h
// Created: Tue Mar  8 13:31:20 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _PaddlesDigiHit_factory_MC_
#define _PaddlesDigiHit_factory_MC_

#include <JANA/JFactoryT.h>
#include "PaddlesDigiHit.h"

#include <map>
#include <utility>

class PaddlesDigiHit_factory_MC:public JFactoryT<PaddlesDigiHit>{
	public:
		PaddlesDigiHit_factory_MC() : JFactoryT<PaddlesDigiHit>("PaddlesDigiHit", "MC"){}
		~PaddlesDigiHit_factory_MC() {}

	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun() {}
		void Finish() {}

		/*This code is here because MC could generate more than 1 hit per sector!*/
		/*The key is the way MC is organized: sector - channel (where sector and channel have a different meaning than in the real data!!!)*/
		std::map<std::pair<int,int>,PaddlesDigiHit*> m_map;
		std::map<std::pair<int,int>,PaddlesDigiHit*>::iterator m_map_it;
};

#endif // _PaddlesDigiHit_factory_MC_

