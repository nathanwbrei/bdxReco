// $Id$
//
//    File: JFactoryGenerator_VETO_INT.h
// Created: Tue Jan 12 11:52:51 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_VETO_INT_
#define _JFactoryGenerator_VETO_INT_

#include <JANA/JFactoryGenerator.h>

#include "IntVetoSiPMHit_factory.h"
#include "IntVetoDigiHit_factory.h"
#include "IntVetoDigiHit_factory_MC.h"
#include "IntVetoHit_factory.h"
#include "IntVetofa250Converter_factory.h"
#include "IntVetoSummary_factory.h"


class JFactoryGenerator_IntVeto: public JFactoryGenerator{
	public:
		JFactoryGenerator_IntVeto(){
			VERBOSE=0;
			japp->SetDefaultParameter("INTVETO:VERBOSE", VERBOSE,
						"Verbosity level for applying messages from INTVETO library."
						" 0=no messages, 10=all messages.");

		}
		virtual ~JFactoryGenerator_IntVeto(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_VETO_INT";}
		
		void GenerateFactories(JFactorySet *factorySet){
			factorySet->Add(new IntVetoSiPMHit_factory());
			factorySet->Add(new IntVetofa250Converter_factory());
			factorySet->Add(new IntVetoDigiHit_factory());
			factorySet->Add(new IntVetoDigiHit_factory_MC());
			factorySet->Add(new IntVetoHit_factory());
			factorySet->Add(new IntVetoSummary_factory());
		}

	private:
		int VERBOSE;
};




#endif // _JFactoryGenerator_VETO_INT_

