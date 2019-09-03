

#ifndef _MCEvent_
#define _MCEvent_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "TObject.h"

#include <TT/TranslationTable.h>

class MCEvent : public JObject, public TObject {
	public:
		JOBJECT_PUBLIC(MCEvent);
		
		void Summarize(JObjectSummary& summary) const final {};

	//	double Ec1,Ec2;
		double E,T, E1, E2, phe1, phe2,E_single_crys;
		int nCalorimeterHits;   //mr
		int nCalorimeterHits_ext_layer;
		int nCalorimeterHits_thr;
		int nCalorimeterHits_S0, nCalorimeterHits_S1, nCalorimeterHits_S2, nCalorimeterHits_S3, nCalorimeterHits_S4, nCalorimeterHits_S5, nCalorimeterHits_S6, nCalorimeterHits_S7;


		vector <TranslationTable::CALO_Index_t> vCalorimeterHits;
		bool flag_RMS;

		int nExtVetoHits;
		int nExtVetoHitsCoincidence;
		vector <TranslationTable::EXT_VETO_Index_t> vExtVetoHits;
		vector <TranslationTable::EXT_VETO_Index_t> vExtVetoHitsCoincidence;

		int nIntVetoHits;
		int nIntVetoHitsCoincidence;
		int nIntVetoHits_0;
		vector <TranslationTable::INT_VETO_Index_t> vIntVetoHits;
		vector <TranslationTable::INT_VETO_Index_t> vIntVetoHitsCoincidence;

		double Ep1,Ep2;

		// For clustering
		double Eseed,Nhit_cluster,Nhit_cluster_near_seed,E_cluster,T_cluster;
		double xseed,yseed, sectorseed;

		double x_crys_max,y_crys_max;
		int Nblock,Block;

		int sector_EM;
		double theta;



		ClassDef(MCEvent,1);

};

#endif // _MCEvent_

