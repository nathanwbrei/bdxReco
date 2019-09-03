// $Id$
//
//    File: CalorimeterHit.h
// Created: Wed Jan 27 16:40:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterHit_
#define _CalorimeterHit_

#include <TT/TranslationTable.h>
#include <system/BDXObject.h>
#include <Calorimeter/CalorimeterSiPMHit.h>

#include <TObject.h>

class TH1D;
class TCanvas;


class CalorimeterHit : public BDXObject{

public:

	CalorimeterHit();

	JOBJECT_PUBLIC(CalorimeterHit);

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) {
		summary.add(m_channel.sector, "sector", "%i");
		summary.add(m_channel.x, "x", "%i");
		summary.add(m_channel.y, "y", "%i");
		summary.add(E, "E", "%f");
		summary.add(T, "T", "%f");
		summary.add(A, "A", "%f");
	}

	TranslationTable::CALO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be ==0
	double E,T;
	double Eraw;
	double A; //amplitude of analogue signal in mV. -1 if not applicable.
	bool RMSflag;


	virtual TCanvas* Draw(int id)const; //!
	mutable TH1D* hWave; //!

	ClassDef(CalorimeterHit,2);
};

#endif // _CalorimeterHit_

