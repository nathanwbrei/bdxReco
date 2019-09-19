#include "CalorimeterHit.h"
#include "TCanvas.h"
#include "TH1D.h"

#include <vector>
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <TT/TranslationTable.h>

CalorimeterHit::CalorimeterHit():
hWave(0){

}

TCanvas* CalorimeterHit::Draw(int id)const{
	int Nwaves;
	int Nsamples;


	vector <const fa250Mode1CalibPedSubHit*> waves;
	//this->Get(waves); //Get those associated to this object
	// TODO: Re-add associated object Get

	Nwaves=waves.size();

	/*Keep the first wave only*/
	hWave=0;
	if (Nwaves>0){
		Nsamples=waves[0]->samples.size();
		hWave=new TH1D(Form("h%i_%i_%i_%i",m_channel.sector,m_channel.x,m_channel.y),Form("h%i_%i_%i_%i",m_channel.sector,m_channel.x,m_channel.y),Nsamples,-0.5*waves[0]->m_dT,(Nsamples-0.5)*waves[0]->m_dT);
		waves[0]->toHisto(hWave);

	}


	if (id>=0){
		if (m_canvas==0) m_canvas=new TCanvas(Form("c%i_%i_%i",m_channel.sector,m_channel.x,m_channel.y),200,200,id);
		m_canvas->Clear();
		m_canvas->cd();
		if (hWave!=0) hWave->Draw();
		return m_canvas;
	}
	else{
		return 0;
	}


}
