// $Id$
//
//    File: CalorimeterCluster.h
// Created: Tue Mar 22 11:19:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterCluster_
#define _CalorimeterCluster_

#include <system/BDXObject.h>
#include "TObject.h"

class TH1D;
class TCanvas;

class CalorimeterCluster: public BDXObject {
private:
	mutable vector<TH1D*> hWaves; //!

public:
	JOBJECT_PUBLIC(CalorimeterCluster)
	;

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {
	    summary.add(m_sector, "sector", "%i");
		summary.add(x, "x", "%f");
		summary.add(y, "y", "%f");
		summary.add(E, "E", "%f");
		summary.add(Eseed, "Eseed", "%f");
		summary.add(T, "T", "%f");
		summary.add(Nhits, "Nhits", "%i");
		summary.add(Nhits_near_seed, "NhitsNearSeed", "%i");
	}

	double Eseed;
	double E, T;
	double x, y;
	int Nhits, Nhits_near_seed, m_sector;

	virtual TCanvas* Draw(int id) const; //!
	ClassDef(CalorimeterCluster,1);

};

#endif // _CalorimeterCluster_

