#include <vector>

#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <TT/TranslationTable.h>
#include "CalorimeterCluster.h"
#include "CalorimeterHit.h"

#include "TCanvas.h"
#include "TH1D.h"


TCanvas* CalorimeterCluster::Draw(int id)const{
	int Nwaves;
	int Nsamples;
	if (m_canvas==0){
		if (id<0){
			m_canvas=new TCanvas(Form("c%i",m_sector),Form("c%i",m_sector),200,200);
		}
		else{
			m_canvas=new TCanvas(Form("c%i",m_sector),200,200,id);
		}
	}
	m_canvas->Clear();
	m_canvas->cd();


	vector <const CalorimeterHit*> chits;
	int ix,iy,idx;
	//this->Get(chits); //Get those associated to this object
	// TODO: Get associated objects doesn't exist at the moment!

	m_canvas->Divide(4,4);



	for (int ihit=0;ihit<chits.size();ihit++){

		ix=chits[ihit]->m_channel.x;
		iy=chits[ihit]->m_channel.y;

		idx=(3-iy)*4+ix+1;

		m_canvas->cd(idx);

		chits[ihit]->Draw(-1);

		if(chits[ihit]->hWave) chits[ihit]->hWave->Draw();
	}


	return m_canvas;
}
