#include "fa250Mode1Hit.h"
#include "TH1D.h"
#include "TCanvas.h"

#include <JANA/JLogger.h>

fa250Mode1Hit::fa250Mode1Hit() :
		hWave(0) {
	m_ped = 0;
	m_RMS = 0;
	m_dT=0;
}

fa250Mode1Hit::~fa250Mode1Hit() {
	if (hWave != 0) {
		delete hWave;
		hWave = 0;
	}
}

TCanvas* fa250Mode1Hit::Draw(int id) const {

	if (m_canvas == 0) {
		if (id < 0) {
			m_canvas = new TCanvas();
		} else {
			m_canvas = new TCanvas(Form("ch%i_%i_%i", m_channel.rocid, m_channel.slot, m_channel.channel), 100, 100, id);
		}
	}
	m_canvas->cd();

	if (hWave != 0) delete hWave;
	hWave = new TH1D(Form("h%i_%i_%i", m_channel.rocid, m_channel.slot, m_channel.channel), Form("h%i_%i_%i", m_channel.rocid, m_channel.slot, m_channel.channel), this->samples.size(), -0.5, this->samples.size() - 0.5);
	this->toHisto(hWave);
	hWave->Draw();
	return m_canvas;
}

void fa250Mode1Hit::toHisto(TH1D *h) const {
	if (h == 0) {
		jerr << "fa250Mode1Hit::toHisto, h pointer is null. Do nothing" << std::endl;
	}
	int N = this->samples.size();
	h->Reset();
	for (int ii = 0; ii < N; ii++) {
		h->Fill(ii, this->samples[ii]);
	}
	for (int ii = 0; ii <= h->GetNbinsX(); ii++) {
		h->SetBinError(ii, 0.);
	}
	return;
}
