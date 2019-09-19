// $Id$
//
//    File: JFactoryGenerator_EPICS.h
// Created: Fri Mar  9 11:23:04 EST 2018
// Creator: celentan (on Darwin celentano-macbook 17.4.0 i386)
//

#ifndef _JFactoryGenerator_EPICS_
#define _JFactoryGenerator_EPICS_

#include <JANA/JFactoryGenerator.h>

#include "epicsData_factory.h"

class JFactoryGenerator_EPICS: public JFactoryGenerator{
	public:
		JFactoryGenerator_EPICS(){}
		virtual ~JFactoryGenerator_EPICS(){}
		virtual const char* className(){return static_className();}
		static const char* static_className(){return "JFactoryGenerator_EPICS";}
		
		void GenerateFactories(JFactorySet *factorySet){
			factorySet->Add(new epicsData_factory());
		}

};

#endif // _JFactoryGenerator_EPICS_

