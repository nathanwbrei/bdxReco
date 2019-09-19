// $Id$
//
//    File: CalorimeterCluster_factory.h
// Created: Tue Mar 22 11:19:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterCluster_factory_
#define _CalorimeterCluster_factory_

#include <JANA/JFactoryT.h>
#include "CalorimeterCluster.h"
#include <vector>
#include <utility>      // std::pair, std::make_pair



class CalorimeterHit;

class CalorimeterCluster_factory:public JFactoryT<CalorimeterCluster>{
	public:
		CalorimeterCluster_factory();
		~CalorimeterCluster_factory(){};



		void setCluster(CalorimeterCluster *cluster,const CalorimeterHit *seed,const std::vector<const CalorimeterHit*> &hits)const;


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun() {}
		void Finish() {}

		double m_CLUSTER_HIT_THR;
		double m_CLUSTER_SEED_THR;
		double m_CLUSTER_POS_W0;
};

#endif // _CalorimeterCluster_factory_

