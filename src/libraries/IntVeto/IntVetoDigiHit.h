// $Id$
//
//    File: IntVetoDigiHit.h
// Created: Wed Jan 20 16:42:38 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoDigiHit_
#define _IntVetoDigiHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <system/BDXObject.h>
#include <TT/TranslationTable.h>
#include "TObject.h"
/*
 * A.C.
 * This object represents a not-yet calibrated it in the inner veto.
 * The goal of this object is dual:
 * - Do the matching of the different IntVetoSiPMhit hits corresponding to the SAME active volume (sector / layer / component)
 * - Be as similar as possible to what is obtained from GEMC, while we wait GEMC producing composite, fadc-like, banks.
 */

class TCanvas;
class TH1D;

class IntVetoDigiHit:public BDXObject{
public:

	JOBJECT_PUBLIC(IntVetoDigiHit);

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {
		summary.add(m_channel.sector, "sector", "%4d");
		summary.add(m_channel.layer, "layer", "%4d");
		summary.add(m_channel.component, "component", "%4d");
		summary.add(m_channel.readout, "readout", "%4d");
		summary.add(Qphe, "Qphe", "%4f");
		summary.add(Qraw, "Qraw", "%4f");
		summary.add(T, "T", "%4f");
		summary.add(A, "A", "%4f");
		summary.add(pedMean, "pedMean", "%4f");
		summary.add(pedRMS, "pedRMS", "%4f");
		summary.add(1*RMSflag, "RMSflag", "%4d");

	}
	//A.C. do not touch these
	TranslationTable::INT_VETO_Index_t m_channel;      //Detector-specific ID. Since this is a detector-based object, the readout field will be ==0

	virtual TCanvas* Draw(int id=0) const;//!  //Exclude from root dictionary
	mutable TH1D* hWave; //!  //Exclude from root dictionary

	double Qphe,Qraw,T,A;
	double pedMean,pedRMS;
	bool RMSflag;


	ClassDef(IntVetoDigiHit,1);

};
#endif // _IntVetoDigiHit_

