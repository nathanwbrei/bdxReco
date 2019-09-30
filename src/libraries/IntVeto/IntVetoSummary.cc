#include "IntVetoSummary.h"
#include "TCanvas.h"
#include "TH1D.h"
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <IntVeto/IntVetoHit.h>
#include <IntVeto/IntVetoSiPMHit.h>



TCanvas* IntVetoSummary::Draw(int id)const{
	int Nwaves;
	int Nsamples;

	int layer,component,readout;

	if (m_canvas==0){
		if (id==0){
			m_canvas=new TCanvas();
		}
		else{
			m_canvas=new TCanvas(Form("c%i_%i_%i",sector),500,500,id);
		}
	}

	vector <const IntVetoHit*> hits;
	vector <const IntVetoSiPMHit*> sipm_hits;

	m_canvas->Clear();
	m_canvas->Divide(3,2);

	/*Get the IntVeto hits*/
	//this->Get(hits);
	// TODO: Fixme

	for (int ihit=0;ihit<hits.size();ihit++){
		layer=hits[ihit]->m_channel.layer;
		component=hits[ihit]->m_channel.component;
		jout<<" "<<layer<<" "<<component<<jendl;
		m_canvas->cd(component+1);
		hits[ihit]->Draw(id)->DrawClonePad();
	}


	return m_canvas;
}
