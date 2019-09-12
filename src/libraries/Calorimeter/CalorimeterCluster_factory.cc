// $Id$
//
//    File: CalorimeterCluster_factory.cc
// Created: Tue Mar 22 11:19:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>

#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>      // std::pair, std::make_pair

using namespace std;

#include "CalorimeterCluster_factory.h"

#include "CalorimeterHit.h"



CalorimeterCluster_factory::CalorimeterCluster_factory(){
	m_CLUSTER_HIT_THR=20;
	m_CLUSTER_SEED_THR=20;
	m_CLUSTER_POS_W0=3.1;
	japp->SetDefaultParameter("CALORIMETER:CLUSTER_HIT_THR",m_CLUSTER_HIT_THR,"Hit threshold in MeV for clustering");
	japp->SetDefaultParameter("CALORIMETER:CLUSTER_SEED_THR",m_CLUSTER_SEED_THR,"Seed threshold in MeV for clustering");
	japp->SetDefaultParameter("CALORIMETER:CLUSTER_POS_W0",m_CLUSTER_POS_W0,"Cluster position algorithm w0 factor");
}


//------------------
// init
//------------------
jerror_t CalorimeterCluster_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterCluster_factory::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterCluster_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	/*Variables required*/
	map<int,vector<const CalorimeterHit*> > sector_hits;  /*key: sector, data: all hits in that sector*/
	map<int,vector<const CalorimeterHit*> >::const_iterator sector_hits_it;  /*key: sector, data: all hits in that sector*/


	vector<const CalorimeterHit*> all_hits;
	vector<const CalorimeterHit*>::const_iterator all_hits_it;
	loop->Get(all_hits);

	double Emax=-99999;
	bool hasSeed=false;
	/*1: divide hits in different sectors. At this stage, just apply the energy cut on a single CRS*/
	for (all_hits_it=all_hits.begin();all_hits_it!=all_hits.end();all_hits_it++){
		if ((*all_hits_it)->E > m_CLUSTER_HIT_THR){
			sector_hits[(*all_hits_it)->m_channel.sector].push_back(*all_hits_it);  //the [] operator in a map create a new data if key does not exists.
		}
	}
	/*2: start the loop on the sectors*/
	for (sector_hits_it=sector_hits.begin();sector_hits_it!=sector_hits.end();sector_hits_it++){
		int this_sector;
		vector<const CalorimeterHit*> this_sector_hits;
		vector<const CalorimeterHit*>::iterator this_sector_hits_it;
		const CalorimeterHit* this_sector_seed;

		this_sector=(sector_hits_it)->first;
		this_sector_hits=(sector_hits_it)->second;


		/*2B: search the seed in this sector*/
		Emax=-99999;
		for (this_sector_hits_it=this_sector_hits.begin();this_sector_hits_it!=this_sector_hits.end();this_sector_hits_it++){
			//Check if the energy of this hit is higher than the maximum. If so, make this as the (temporary) maximum
			if ((*this_sector_hits_it)->E>Emax){
				Emax=(*this_sector_hits_it)->E;
				this_sector_seed=(*this_sector_hits_it);
			}
		}
		hasSeed=false;
		//At the end of this loop, this_sector_seed MAY be a seed, if the energy is higher than the CLUSTER_SEED_THR
		 if (this_sector_seed->E>m_CLUSTER_SEED_THR){
			hasSeed=true;
		}
		//If there is NO seed, then in this sector there is NO EM shower. Go on, to the next sector
		if (hasSeed==false) continue;
		else{ //There is a cluster
			CalorimeterCluster *cluster = new CalorimeterCluster;        //Create a cluster
			this->setCluster(cluster,this_sector_seed,this_sector_hits); //set all the cluster properties (see below)
			_data.push_back(cluster); 							         //publish it
		}
	}//end loop on sectors




	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CalorimeterCluster_factory::erun(void)
{
	return NOERROR;
}


//------------------
// fini
//------------------
jerror_t CalorimeterCluster_factory::fini(void)
{
	return NOERROR;
}

void CalorimeterCluster_factory::setCluster(CalorimeterCluster *cluster,const CalorimeterHit *seed,const std::vector<const CalorimeterHit*> &hits)const{
	std::vector<const CalorimeterHit*>::const_iterator hits_it;
	double Etot;
	double E,f;
	double pos_weight;

	double den,Xnum,Ynum;
	den=0;
	Xnum=0;
	Ynum=0;

	cluster->Nhits = 0;
	cluster->Nhits_near_seed = 0;
	cluster->E = 0;             			 //Set the cluster total energy
	cluster->Eseed = (seed)->E;              //Set the seed energy to the cluster
	cluster->m_sector= seed->m_channel.sector;

	for (hits_it=hits.begin();hits_it!=hits.end();hits_it++){ //set the cluster total energy. Add the hits to the cluster
		E=(*hits_it)->E;
		cluster->E += E;
		cluster->Nhits++;
		if(abs((*hits_it)->m_channel.x-seed->m_channel.x)<=1 && abs((*hits_it)->m_channel.y-seed->m_channel.y)<=1 )cluster->Nhits_near_seed++;
		cluster->AddAssociatedObject(*hits_it);
	}
	Etot=cluster->E;

	for (hits_it=hits.begin();hits_it!=hits.end();hits_it++){ //weighted x-y
		E=(*hits_it)->E;
		pos_weight=std::max(0.,(m_CLUSTER_POS_W0+log(E/Etot)));
		//	den+=E*pos_weight;
		den+=pos_weight;  // from HPS note 2014-002

		Xnum+=((*hits_it)->m_channel.x)*pos_weight;
		Ynum+=((*hits_it)->m_channel.y)*pos_weight;
	}
	cluster->x=(Xnum/den);
	cluster->y=(Ynum/den);
}








