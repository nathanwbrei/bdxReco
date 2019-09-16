// $Id$
//
//    File: fa250Mode1CalibPedSubHitFFT_factory.cc
// Created: Mon May 27 15:32:26 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#include "fa250Mode1CalibPedSubHit.h"
#include "fa250Mode1CalibPedSubHitFFT_factory.h"
#include "TVirtualFFT.h"

#include <iostream>
#include <iomanip>


//------------------
// evnt
//------------------
void fa250Mode1CalibPedSubHitFFT_factory::Process(const std::shared_ptr<const JEvent>& event) {
	// Code to generate factory data goes here. Add it like:
	//
	// fa250Mode1CalibPedSubHitFFT *myfa250Mode1CalibPedSubHitFFT = new fa250Mode1CalibPedSubHitFFT;
	// myfa250Mode1CalibPedSubHitFFT->x = x;
	// myfa250Mode1CalibPedSubHitFFT->y = y;
	// ...
	// _data.push_back(myfa250Mode1CalibPedSubHitFFT);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	vector<const fa250Mode1CalibPedSubHit*> fa250Mode1CalibPedSubHits;
	event->Get(fa250Mode1CalibPedSubHits);
	for (uint32_t i = 0; i < fa250Mode1CalibPedSubHits.size(); i++) {

		const fa250Mode1CalibPedSubHit *hit = fa250Mode1CalibPedSubHits[i];

		// Create new fa250Mode1PedSubHitFFT
		fa250Mode1CalibPedSubHitFFT *CalibPedSubHitFFT = new fa250Mode1CalibPedSubHitFFT;

		// Copy the fa250Hit part (crate, slot, channel, ...)
		// doing it this way allow one to modify fa250 later and
		// not have to change this code.
		fa250Hit *a = CalibPedSubHitFFT;
		const fa250Hit *b = hit;
		*a = *b;

		CalibPedSubHitFFT->m_dT = hit->m_dT;
		CalibPedSubHitFFT->m_ped = hit->m_ped;
		CalibPedSubHitFFT->m_RMS = hit->m_RMS;

		//Here do FFT
		int n = hit->samples.size();
		Double_t *in = new Double_t[2 * (n / 2 + 1)];
		Double_t *re_full = new Double_t[n];
		Double_t *im_full = new Double_t[n];

		//Set input
		for (int jj = 0; jj < n; jj++) {
			in[jj] = hit->samples[jj];
		}
		//Do FFT
		TVirtualFFT *fft_own = TVirtualFFT::FFT(1, &n, "R2C ES K");
		fft_own->SetPoints(in);
		fft_own->Transform();
		fft_own->GetPointsComplex(re_full,im_full);
		delete [] in;

		//FILTER
		for (int jj = 0; jj < n; jj++) {
			re_full[jj] /= n;
			im_full[jj] /= n;
			if (jj > 20) {
				re_full[jj] = 0;
				im_full[jj] = 0;
			}
		}

		//Go Back
		TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &n, "C2R M K");
		fft_back->SetPointsComplex(re_full, im_full);
		fft_back->Transform();

		in=fft_back->GetPointsReal();



		for (uint32_t jj = 0; jj < hit->samples.size(); jj++) {  //j=0
			CalibPedSubHitFFT->samples.push_back(in[jj]);
		}

		delete [] re_full;
		delete [] im_full;
		delete fft_own;
		delete fft_back;

		// Add original as associated object
		CalibPedSubHitFFT->AddAssociatedObject(hit);
		Insert(CalibPedSubHitFFT);
	}
}

