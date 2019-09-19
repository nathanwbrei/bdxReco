// $Id$
//
//    File: JFactoryGenerator_VETO_INT.h
// Created: Tue Jan 12 11:52:51 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_CALORIMETER_
#define _JFactoryGenerator_CALORIMETER_

#include <JANA/JFactoryGenerator.h>

#include "CalorimeterSiPMHit_factory.h"
#include "CalorimeterDigiHit_factory.h"
#include "CalorimeterDigiHit_factory_MC.h"
#include "CalorimeterHit_factory.h"
#include "CalorimeterCluster_factory.h"
#include "Calorimeterfa250Converter_factory.h"
#include "CalorimeterMCRealHit_factory.h"


class JFactoryGenerator_Calorimeter: public JFactoryGenerator{
	public:
		JFactoryGenerator_Calorimeter(){
			VERBOSE=0;
			japp->SetDefaultParameter("CALORIMETER:VERBOSE", VERBOSE,
						"Verbosity level for applying messages from CALORIMETER library."
						" 0=no messages, 10=all messages.");

		}
		virtual ~JFactoryGenerator_Calorimeter(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_CALORIMETER";}
		
		void GenerateFactories(JFactorySet *factorySet){
			factorySet->Add(new CalorimeterSiPMHit_factory());
			factorySet->Add(new Calorimeterfa250Converter_factory());
			factorySet->Add(new CalorimeterDigiHit_factory());
			factorySet->Add(new CalorimeterDigiHit_factory_MC());
			factorySet->Add(new CalorimeterHit_factory());
			factorySet->Add(new CalorimeterMCRealHit_factory());
			factorySet->Add(new CalorimeterCluster_factory());
		}

	private:
		int VERBOSE;
};




#endif // _JFactoryGenerator_VETO_INT_
