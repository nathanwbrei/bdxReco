// $Id$
//
//    File: JFactoryGenerator_DAQ.h
// Created: Tue Jan 12 11:28:22 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_BDXmini_
#define _JFactoryGenerator_BDXmini_

#include <JANA/JFactoryGenerator.h>

#include "triggerDataBDXmini_factory.h"


class JFactoryGenerator_BDXmini: public JFactoryGenerator {
public:
	JFactoryGenerator_BDXmini() {
	}
	virtual ~JFactoryGenerator_BDXmini() {
	}
	virtual const char* className(void) {
		return static_className();
	}
	static const char* static_className(void) {
		return "JFactoryGenerator_BDXmini";
	}

	void GenerateFactories(JFactorySet *factorySet) {
		factorySet->Add(new triggerDataBDXmini_factory());
	}

};

#endif // _JFactoryGenerator_DAQ_

