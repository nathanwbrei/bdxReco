// $Id$
//
//    File: JEventProcessor_Calorimeter_rate.cc
// Created: Fri Mar 25 18:38:03 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "JEventProcessor_Calorimeter_rate.h"
#include <system/BDXEventProcessor.h>

#include <DAQ/eventData.h>
#include <DAQ/fa250Mode1CalibPedSubHit.h>

#include <TT/TranslationTable.h>


#include <Calorimeter/CalorimeterHit.h>
#include <IntVeto/IntVetoHit.h>
#include <IntVeto/IntVetoSummary.h>

#include <system/JROOTOutput.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TTree.h"


using namespace jana;

using namespace std;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_Calorimeter_rate());
}
} // "C"


void *waveTohisto(const fa250Mode1Hit *hit,TH1D *h);

//------------------
// JEventProcessor_Calorimeter_rate (Constructor)
//------------------
JEventProcessor_Calorimeter_rate::JEventProcessor_Calorimeter_rate()
{

}

//------------------
// ~JEventProcessor_Calorimeter_rate (Destructor)
//------------------
JEventProcessor_Calorimeter_rate::~JEventProcessor_Calorimeter_rate()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_Calorimeter_rate::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//
	japp->RootWriteLock();
	t=new TTree("CaloRate","CaloRate");
	t->Branch("eventN",&eventNumber);
	t->Branch("Ec",&Ec);
	t->Branch("nHits",&nHits);

	h=new TH1D*[100];
	for (int ii=0;ii<100;ii++){
		h[ii]=new TH1D("h","h",500,0,500);
	}

	japp->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_Calorimeter_rate::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes

	if (m_isFirstCallToBrun){
		string class_name,this_class_name;
		string joutput_name;
		BDXEventProcessor *m_BDXEventProcessor;
		vector<JEventProcessor*> m_processors=app->GetProcessors();
		vector<JEventProcessor*>::iterator m_processors_it;

		m_isFirstCallToBrun=0;
		class_name="BDXEventProcessor";
		joutput_name="JROOTOutput";
		//Now I need to determine which processor is the one holding the output. Discussing with David, he suggested just to check the class name, since
		//a dynamic cast may not work with plugins
		for (m_processors_it=m_processors.begin();m_processors_it!=m_processors.end();m_processors_it++){
			if ((*m_processors_it)!=0){
				this_class_name=string((*m_processors_it)->className());
				if (this_class_name==class_name){
					m_BDXEventProcessor=(BDXEventProcessor*)(*m_processors_it);
					if (m_BDXEventProcessor->getOutput()==0){
						jerr<<"BDXEventProcessor JOutput is null!"<<endl;
						break;
					}
					if (string(m_BDXEventProcessor->getOutput()->className())==joutput_name){
						m_ROOTOutput=(JROOTOutput*)(m_BDXEventProcessor->getOutput());
						jout<<"Got JROOTOutput!"<<endl;
					}
					else{
						jerr<<"BDXEventProcessor JOutput is not null BUT class is: "<<m_BDXEventProcessor->getOutput()->className()<<endl;
					}
				}
			}
		}
		/*For ALL objects you want to add to ROOT file, use the following:*/
		if (m_ROOTOutput){
			m_ROOTOutput->AddObject(t);
			for (int ii=0;ii<100;ii++){
				m_ROOTOutput->AddObject(h[ii]);
			}
		}
	}





	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_Calorimeter_rate::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	vector<const CalorimeterHit*> chits;
	vector<const CalorimeterHit*>::iterator chits_it;

	vector<const IntVetoHit*> ivhits;
	vector<const IntVetoHit*>::iterator ivhits_it;

	vector <const fa250Mode1CalibPedSubHit*> waves;
	vector <const fa250Mode1CalibPedSubHit*>::iterator waves_it;

	vector <const IntVetoSummary*> IntVetoSum;
	vector <const IntVetoSummary*>::iterator IntVetoSum_it;


	const eventData* evdata;

	const IntVetoSummary* IntVetoSum0;

	int sector,layer,component;
	int crate,slot,channel;
	int N;
	bool flag;

	loop->Get(chits);
	loop->Get(ivhits);
	loop->Get(waves);
	loop->Get(IntVetoSum);
	try{
		loop->GetSingle(evdata);
	}
	catch(unsigned long e){
		jout<<"No trig bank this event"<<endl;
		return 	OBJECT_NOT_AVAILABLE;
	}

	int tWord=evdata->triggerWords[0];
	if ((tWord&0x1)==0) return NOERROR;
	if (chits.size()!=1) return RESOURCE_UNAVAILABLE;


	japp->RootWriteLock();
	flag=false;
	nHits=0;
	eventNumber=eventnumber;

	if (IntVetoSum.size()==1){
		IntVetoSum0=IntVetoSum[0];
		nHits=IntVetoSum0->nHits;
	}
	else{
		nHits=0;
	}
	Ec=chits[0]->E;

	if ((nHits==0)&&(Ec>20)){

		N=0;
		for (waves_it=waves.begin();waves_it!=waves.end();waves_it++){
			(*waves_it)->toHisto(h[N]);
			h[N]->SetName(Form("%i___%i_%i",eventnumber,(*waves_it)->m_channel.slot,(*waves_it)->m_channel.channel));
			h[N]->SetTitle(Form("%i___%i_%i",eventnumber,(*waves_it)->m_channel.slot,(*waves_it)->m_channel.channel));
			h[N]->Write();
			N++;
		}
	}


	t->Fill();


	japp->RootUnLock();


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_Calorimeter_rate::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_Calorimeter_rate::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}






