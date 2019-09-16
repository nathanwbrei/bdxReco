// $Id$
//
//    File: fa250Mode1CalibHit.h
// Created: Wed Feb  3 07:38:15 EST 2016
// Creator: davidl (on Darwin harriet.jlab.org 13.4.0 i386)
//

#ifndef _fa250Mode1CalibPedSubHit_
#define _fa250Mode1CalibPedSubHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include "fa250Mode1Hit.h"

class fa250Mode1CalibPedSubHit:public fa250Mode1Hit{
	public:
		JOBJECT_PUBLIC(fa250Mode1CalibPedSubHit);
		fa250Mode1CalibPedSubHit();
		virtual ~fa250Mode1CalibPedSubHit();

		
		void Summarize(JObjectSummary& summary) const override {
			 summary.add(m_channel.rocid, "crate", "%4d");
			 summary.add(m_channel.slot, "slot", "%4d");
			 summary.add(m_channel.channel, "channel", "%4d");
			 summary.add(samples.size(), "nsamples", "%4d");
		}
		
		double m_ped,m_RMS; //from DB


		virtual TCanvas* Draw(int id=0) const;
		void toHisto(TH1D *h) const;

		ClassDef(fa250Mode1CalibPedSubHit,1);
};

#endif // _fa250Mode1CalibHit_

