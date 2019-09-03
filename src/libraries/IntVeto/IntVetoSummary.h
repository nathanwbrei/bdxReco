// $Id$
//
//    File: IntVetoSummary.h
// Created: Tue Mar 29 15:07:39 CEST 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoSummary_
#define _IntVetoSummary_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>

#include <system/BDXObject.h>

class TCanvas;
class TH1D;

class IntVetoSummary:public BDXObject{
	public:
		JOBJECT_PUBLIC(IntVetoSummary);

		void Summarize(JObjectSummary& summary) const final {
			 summary.add(sector, "sector", "%4d");
			 summary.add(hits.size(), "Nhits", "%4d");
			 for (int ii=0;ii<hits.size();ii++){
				 summary.add(hits[ii].layer, "component hit layer", "%4d");
				 summary.add(hits[ii].component, "component hit component", "%4d");
			 }
		}
		int sector;
		
		int nHits;
		vector<TranslationTable::INT_VETO_Index_t> hits;


		virtual TCanvas* Draw(int id)const;

	private:
		vector <TH1D*> hWaves;
};

#endif // _IntVetoSummary_

