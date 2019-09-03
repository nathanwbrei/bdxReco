// $Id$
//
//    File: CataniaEvent.h
// Created: Mon Apr 25 23:25:44 CEST 2016
// Creator: celentan (on Linux dhcp-visitor-enr-116-196.slac.stanford.edu 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _CataniaEvent_
#define _CataniaEvent_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include <TT/TranslationTable.h>

#include "TObject.h"

class CataniaEvent : public JObject, public TObject {
public:
	JOBJECT_PUBLIC(CataniaEvent);

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {
		summary.add(runN, "runN", "%i");
		summary.add(eventN, "eventN", "%i");
		summary.add(time, "time", "%i");
		summary.add((long long) timestamp, "timestamp", "%lld");
		summary.add(tWord, "tWord", "%i");
		summary.add(1*flag_RMS, "RMSflag", "%d");
		summary.add(1*flag_RMS1, "RMSflag1", "%d");
		summary.add(1*flag_RMS2, "RMSflag2", "%d");
		summary.add(E1, "E1", "%f");
		summary.add(E2, "E2", "%f");
		summary.add(T1, "T1", "%f");
		summary.add(T2, "T2", "%f");
	}

	double Qc1, Qc2, Qc3, Qcs1, Qcs2, Qcs3;
	double Ec1, Ec2, Ec3;

	double E1, T1, E2, T2, E, T;
	bool flag_RMS, flag_RMS1, flag_RMS2;

	int nExtVetoHits;
	int nExtVetoHitsCoincidence;
	vector<TranslationTable::EXT_VETO_Index_t> vExtVetoHits;
	vector<bool> vExtVetoHitsIsInCoincidence;
	vector<double> vExtVetoHitsT;
	vector<double> vExtVetoHitsE;

	int nIntVetoHits;
	int nIntVetoHitsCoincidence;
	vector<TranslationTable::INT_VETO_Index_t> vIntVetoHits;
	vector<bool> vIntVetoHitsIsInCoincidence;
	vector<double> vIntVetoHitsT;
	vector<double> vIntVetoHitsQ;

	double Ep1, Ep2;

	int time; //in s
	int tWord;

	uint64_t eventN;
	int runN;

	uint64_t timestamp;

	ClassDef(CataniaEvent,1);

};

#endif // _CataniaEvent_

