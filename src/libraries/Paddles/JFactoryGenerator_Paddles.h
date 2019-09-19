// $Id$
//
//    File: JFactoryGenerator_ExtVeto.h
// Created: Wed Jan 13 21:06:57 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_Paddles_
#define _JFactoryGenerator_Paddles_

#include <JANA/JFactoryGenerator.h>

#include "Paddlesfa250Converter_factory.h"
#include "PaddlesPMTHit_factory.h"
#include "PaddlesDigiHit_factory.h"
#include "PaddlesDigiHit_factory_MC.h"
#include "PaddlesHit_factory.h"

class JFactoryGenerator_Paddles: public JFactoryGenerator{
	public:
		JFactoryGenerator_Paddles(){
			VERBOSE=0;
						japp->SetDefaultParameter("PADDLES:VERBOSE", VERBOSE,
									"Verbosity level for applying messages from PADDLES library."
									" 0=no messages, 10=all messages.");
		}
		virtual ~JFactoryGenerator_Paddles(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_Paddles";}
		
		void GenerateFactories(JFactorySet *factorySet){
			factorySet->Add(new Paddlesfa250Converter_factory());
			factorySet->Add(new PaddlesPMTHit_factory());
			factorySet->Add(new PaddlesDigiHit_factory());
			factorySet->Add(new PaddlesDigiHit_factory_MC());
			factorySet->Add(new PaddlesHit_factory());
		}

	private:
		int VERBOSE;

};

#endif // _JFactoryGenerator_Paddles_

