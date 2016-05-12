
#include <iostream>
#include <iomanip>
using namespace std;

#include "MCEvent_EM_factory.h"

#include <Calorimeter/CalorimeterHit.h>
#include <Calorimeter/CalorimeterCluster.h>
#include <IntVeto/IntVetoHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <Paddles/PaddlesHit.h>


using namespace jana;

//------------------
// init
//------------------
jerror_t MCEvent_EM_factory::init(void)
{

	m_EC2_cut=20; //MeV
	m_ExtVeto_timeWindows=250; //ns
	m_IntVeto_timeWindows=250; //ns
	gPARMS->SetDefaultParameter("CATANIAEVENT:EC2_CUT",m_EC2_cut,"Cut on MPPC#2 energy in MeV");
	gPARMS->SetDefaultParameter("CATANIAEVENT:EXTVETO_TIME_WINDOW",m_ExtVeto_timeWindows,"ExtVeto time window (ns)");
	gPARMS->SetDefaultParameter("CATANIAEVENT:INTVETO_TIME_WINDOW",m_ExtVeto_timeWindows,"IntVeto time window (ns)");




	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t MCEvent_EM_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t MCEvent_EM_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{



	vector <const CalorimeterHit*> chits;
	vector <const CalorimeterHit*>::const_iterator chits_it;

	vector <const CalorimeterCluster*> cclusters;
	vector <const CalorimeterCluster*>::const_iterator cclusters_it;

	vector <const IntVetoHit*> ivhits;
	vector <const IntVetoHit*>::const_iterator ivhits_it;

	vector <const ExtVetoHit*> evhits;
	vector <const ExtVetoHit*>::const_iterator evhits_it;



	loop->Get(chits);
	loop->Get(cclusters);
	loop->Get(ivhits);
	loop->Get(evhits);



	MCEvent_EM *m_event=new MCEvent_EM();
	m_event->E=0;
	m_event->E1=0;
	m_event->E2=0;
	m_event->phe1=0;
	m_event->phe2=0;
	m_event->T=0;
	m_event->nCalorimeterHits=0;

	m_event->nCalorimeterHits_S0=0;
	m_event->nCalorimeterHits_S1=0;
	m_event->nCalorimeterHits_S2=0;
	m_event->nCalorimeterHits_S3=0;
	m_event->nCalorimeterHits_S4=0;
	m_event->nCalorimeterHits_S5=0;
	m_event->nCalorimeterHits_S6=0;
	m_event->nCalorimeterHits_S7=0;

	m_event->Eseed=0;
	m_event->xseed=0;
	m_event->yseed=0;
	m_event->Nhit_cluster=0;
	m_event->E_cluster=0;
	m_event->T_cluster=0;


	m_event->flag_RMS=false;
	double T1,T2,dT;
	bool flag1,flag2;

	T1=T2=0;
	flag1=false;
	flag2=false;

	for (chits_it=chits.begin();chits_it!=chits.end();chits_it++){			// loop over the fired crystals
		const CalorimeterHit *hit=(*chits_it);

		if(hit->T<0) continue;
		else{
		for (int ihit=0;ihit<hit->m_data.size();ihit++){			// loop over the 2 SiPMs
				switch (hit->m_data[ihit].readout){
				case 1:
					m_event->phe1 +=(hit->m_data[ihit].Q);   	// Q-> number of p.e.
					m_event->E1=(hit->m_data[ihit].Q)/7.3; 		// energy calibration from 20 MeV simulated protons
					break;
				case 2:
					m_event->phe2 +=(hit->m_data[ihit].Q);  	// Q-> number of p.e.
					m_event->E2=(hit->m_data[ihit].Q)/14.6; 	// energy calibration from 20 MeV simulated protons
					break;
				}
			}


		m_event->E += (m_event->E1 +m_event->E2)/2;				// sum the energies of all the crystals fired
	    m_event->nCalorimeterHits++;
	    m_event->vCalorimeterHits.push_back(hit->m_channel);

	    switch (hit->m_channel.sector){
	     case 0:
	    	 m_event->nCalorimeterHits_S0++;
	    	 break;

	     case 1:
	    	  m_event->nCalorimeterHits_S1++;
	    	  break;

	     case 2:
	    	  m_event->nCalorimeterHits_S2++;
	    	  break;

	     case 3:
	    	  m_event->nCalorimeterHits_S3++;
	    	  break;

	     case 4:
	    	  m_event->nCalorimeterHits_S4++;
	    	  break;

	     case 5:
	    	  m_event->nCalorimeterHits_S5++;
	    	  break;

	     case 6:
	    	  m_event->nCalorimeterHits_S6++;
	    	  break;

	     case 7:
	    	  m_event->nCalorimeterHits_S7++;
	    	  break;

	                                   }

		            }   //end else
	              }			// end loop over the fired crystals



 //          jout << "S0= "<< m_event->nCalorimeterHits_S0<<endl;
 //          jout << "S1= "<< m_event->nCalorimeterHits_S1<<endl;
 //          jout << "S2= "<< m_event->nCalorimeterHits_S2<<endl;
 //          jout << "S3= "<< m_event->nCalorimeterHits_S3<<endl;
 //          jout << "S4= "<< m_event->nCalorimeterHits_S4<<endl;
 //          jout << "S5= "<< m_event->nCalorimeterHits_S5<<endl;
 //          jout << "S6= "<< m_event->nCalorimeterHits_S6<<endl;
 //          jout << "S7= "<< m_event->nCalorimeterHits_S7<<endl;



	for (cclusters_it=cclusters.begin();cclusters_it!=cclusters.end();cclusters_it++){			// loop over clusters
			const CalorimeterCluster *hit=(*cclusters_it);

//	       jout << "Eseed "<< hit->Eseed<<endl;
//           jout << "x " << hit->x<<endl;
//           jout << "y "<<hit->y<<endl;
//           jout << "Nhit "<<hit->Nhits<<endl;
//           jout << "E "<< hit->E<<endl;

			m_event->Eseed=hit->Eseed;
			m_event->xseed=hit->x;
			m_event->yseed=hit->y;
			m_event->Nhit_cluster=hit->Nhits;
			m_event->E_cluster=hit->E;
			m_event->T_cluster;

	}





	/*Now loop on external veto hits*/
	m_event->nExtVetoHits=0;
	m_event->nExtVetoHitsCoincidence=0;

	for (evhits_it=evhits.begin();evhits_it!=evhits.end();evhits_it++){			// loop over the fired EV
		const ExtVetoHit *hit=(*evhits_it);

//		jout << "Sector= "<<hit->m_channel.sector<< "Component  "<<hit->m_channel.component<<endl;

		if (hit->T<0) continue; //The ExtVeto condition for a "good" hit

		else{
			m_event->nExtVetoHits++;
			m_event->vExtVetoHits.push_back(hit->m_channel);

			 // if (hit->m_channel.component>6){
				//		jout << hit->m_channel.sector<< " "<<hit->m_channel.component<<endl;
				//		}



		}
	}
	/*Now loop on inner veto hits*/
	m_event->nIntVetoHits=0;
	m_event->nIntVetoHitsCoincidence=0;
	for (ivhits_it=ivhits.begin();ivhits_it!=ivhits.end();ivhits_it++){
		const IntVetoHit *hit=(*ivhits_it);

		if (hit->T<0) continue; //The IntVeto condition for a "good" hit
		else{
			m_event->nIntVetoHits++;
			m_event->vIntVetoHits.push_back(hit->m_channel);

		}
	}

	_data.push_back(m_event);

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t MCEvent_EM_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t MCEvent_EM_factory::fini(void)
{
	return NOERROR;
}

