// $Id$
//
//    File: JFactoryGenerator_ExtVeto.h
// Created: Wed Jan 13 21:06:57 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_ExtVeto_
#define _JFactoryGenerator_ExtVeto_

#include <JANA/JFactoryGenerator.h>
#include "ExtVetofa250Converter_factory.h"
#include "ExtVetoPMTHit_factory.h"
#include "ExtVetoDigiHit_factory.h"
#include "ExtVetoDigiHit_factory_MC.h"
#include "ExtVetoHit_factory.h"
#include "ExtVetoSummary_factory.h"

class JFactoryGenerator_ExtVeto: public JFactoryGenerator{
	public:
		JFactoryGenerator_ExtVeto(){

			VERBOSE=0;
					japp->SetDefaultParameter("EXTVETO:VERBOSE", VERBOSE,
								"Verbosity level for applying messages from INTVETO library."
								" 0=no messages, 10=all messages.");




		}
		virtual ~JFactoryGenerator_ExtVeto(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_ExtVeto";}
		
		void GenerateFactories(JFactorySet *factorySet){
			factorySet->Add(new ExtVetoPMTHit_factory());
			factorySet->Add(new ExtVetofa250Converter_factory());
			factorySet->Add(new ExtVetoDigiHit_factory());
			factorySet->Add(new ExtVetoDigiHit_factory_MC());
			factorySet->Add(new ExtVetoHit_factory());
			factorySet->Add(new ExtVetoSummary_factory());
		}
	private:
			int VERBOSE;

};

#endif // _JFactoryGenerator_ExtVeto_

