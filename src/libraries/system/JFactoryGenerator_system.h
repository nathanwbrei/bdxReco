// $Id$
//
//    File: JFactoryGenerator_ExtVeto.h
// Created: Wed Jan 13 21:06:57 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_system_
#define _JFactoryGenerator_system_

#include <JANA/JFactoryGenerator.h>



class JFactoryGenerator_system: public JFactoryGenerator{
	public:
		JFactoryGenerator_system(){
			VERBOSE=0;
			japp->SetDefaultParameter("SYSTEM:VERBOSE", VERBOSE,
					"Verbosity level for applying messages from system library."
					" 0=no messages, 10=all messages.");
		}
		virtual ~JFactoryGenerator_system(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_system";}
		
		void GenerateFactories(JFactorySet *factorySet){
		}

	private:
		int VERBOSE;

};

#endif // _JFactoryGenerator_system_

