// $Id$
//
//    File: JFactoryGenerator_EventBuilder.h
// Created: Tue Apr 26 00:28:27 CEST 2016
// Creator: celentan (on Linux dhcp-visitor-enr-116-196.slac.stanford.edu 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_EventBuilder_
#define _JFactoryGenerator_EventBuilder_

#include <JANA/JFactoryGenerator.h>


//#include "MCEvent_factory.h"
#include "CataniaEvent_factory.h"
#include "TEvent_factory_CataniaProto2.h"
#include "TEvent_factory_JLabFlux.h"
#include "TEvent_factory_BDXmini.h"

#ifdef MC_SUPPORT_ENABLE
#include "TEvent_factory_FullMC.h"
#endif

class JFactoryGenerator_EventBuilder: public JFactoryGenerator {
public:
	JFactoryGenerator_EventBuilder() {
	}
	virtual ~JFactoryGenerator_EventBuilder() {
	}
	virtual const char* className(void) {
		return static_className();
	}
	static const char* static_className(void) {
		return "JFactoryGenerator_EventBuilder";
	}

	void GenerateFactories(JFactorySet *factorySet) {

		factorySet->Add(new CataniaEvent_factory());
		factorySet->Add(new TEvent_factory_CataniaProto2());
		factorySet->Add(new TEvent_factory_JLabFlux());
		factorySet->Add(new TEvent_factory_BDXmini());
#ifdef MC_SUPPORT_ENABLE
		factorySet->Add(new TEvent_factory_FullMC());
#endif
	}

};

#endif // _JFactoryGenerator_EventBuilder_

