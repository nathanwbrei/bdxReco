#include "fa250Mode1CalibPedSubHitFFT.h"
#include "TH1D.h"
#include "TCanvas.h"

fa250Mode1CalibPedSubHitFFT::fa250Mode1CalibPedSubHitFFT(){
}

fa250Mode1CalibPedSubHitFFT::~fa250Mode1CalibPedSubHitFFT(){

}

TCanvas* fa250Mode1CalibPedSubHitFFT::Draw(int id) const{


	if (m_canvas==0){
		if (id<0){
			m_canvas=new TCanvas();
		}
		else{
			m_canvas=new TCanvas(Form("ch%i_%i_%i",m_channel.rocid,m_channel.slot,m_channel.channel),100,100,id);
		}
	}
	m_canvas->cd();

	if (hWave!=0) delete hWave;
	hWave=new TH1D(Form("hhh%i_%i_%i",(int)m_channel.rocid,(int)m_channel.slot,(int)m_channel.channel),Form("h%i_%i_%i",(int)m_channel.rocid,(int)m_channel.slot,(int)m_channel.channel),this->samples.size(),-0.5*m_dT,(this->samples.size()-0.5)*m_dT);
	this->toHisto(hWave);
	hWave->Draw();
	return m_canvas;
}


void fa250Mode1CalibPedSubHitFFT::toHisto(TH1D *h)const{
	if (h==0){
		jerr<<"fa250Mode1CalibPedSubHitFFT::toHisto, h pointer is null. Do nothing"<<jendl;
	}
	int N=this->samples.size();
	h->Reset();
	for (int ii=0;ii<N;ii++){
		h->Fill(ii*m_dT,this->samples[ii]);
	}
	for (int ii=0;ii<=h->GetNbinsX();ii++){
		h->SetBinError(ii,0.);
	}
	return;
}
