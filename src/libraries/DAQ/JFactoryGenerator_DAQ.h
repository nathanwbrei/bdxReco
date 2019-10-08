// $Id$
//
//    File: JFactoryGenerator_DAQ.h
// Created: Tue Jan 12 11:28:22 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_DAQ_
#define _JFactoryGenerator_DAQ_

#include <JANA/JFactoryGenerator.h>

#include "fa250Mode1Hit.h"
#include "fa250Mode7Hit.h"
#include "fa250Mode1CalibPedSubHit_factory.h"
#include "fa250Mode1CalibPedSubHitFFT_factory.h"
#include "eventData.h"
#include "epicsRawData.h"
#include "fa250WaveboardV1Hit.h"

class JFactoryGenerator_DAQ: public JFactoryGenerator {
public:
	JFactoryGenerator_DAQ() {
	}
	virtual ~JFactoryGenerator_DAQ() {
	}
	virtual const char* className(void) {
		return static_className();
	}
	static const char* static_className(void) {
		return "JFactoryGenerator_DAQ";
	}

	void GenerateFactories(JFactorySet* factorySet) {
		factorySet->Add(new JFactoryT<fa250WaveboardV1Hit>());
// "Dummy factories" corresponding to items acquired via JEventSource::GetObjects no longer get a FactoryGenerator
//		factorySet->Add(new JFactoryT<fa250Mode1Hit>());
//		factorySet->Add(new JFactoryT<fa250Mode7Hit>());
//		factorySet->Add(new JFactoryT<eventData>());
//		factorySet->Add(new JFactoryT<epicsRawData>());
		factorySet->Add(new fa250Mode1CalibPedSubHit_factory());
		factorySet->Add(new fa250Mode1CalibPedSubHitFFT_factory());
	}

};

#endif // _JFactoryGenerator_DAQ_

