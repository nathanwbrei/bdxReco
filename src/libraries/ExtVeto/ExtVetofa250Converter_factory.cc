// $Id$
//
//    File: ExtVetofa250Converter_factory.cc
// Created: Mon Jan 25 16:28:59 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetofa250Converter_factory.h"

//------------------
// init
//------------------
void ExtVetofa250Converter_factory::Init()
{
	m_extVetofa250Converter=new ExtVetofa250Converter();
	m_extVetofa250Converter->threshold=new CalibrationHandler<TranslationTable::EXT_VETO_Index_t>("/ExtVeto/Threshold");
	this->mapCalibrationHandler(m_extVetofa250Converter->threshold);

	m_extVetofa250Converter->m_pedestals=new DAQCalibrationHandler("/DAQ/pedestals");
	this->mapCalibrationHandler(m_extVetofa250Converter->m_pedestals);

	return NOERROR;
}

//------------------
// brun
//------------------
void ExtVetofa250Converter_factory::ChangeRun(const std::shared_ptr<const JEvent>& event)
{
	if (m_isFirstCallToBrun){
		m_isFirstCallToBrun=0;
		_data.push_back(m_extVetofa250Converter);
		SetFactoryFlag(PERSISTANT);
	}


	this->updateCalibrationHandler(m_extVetofa250Converter->threshold,eventLoop);
	this->updateCalibrationHandler(m_extVetofa250Converter->m_pedestals,eventLoop);




	return NOERROR;
}

//------------------
// evnt
//------------------
void ExtVetofa250Converter_factory::Process(const std::shared_ptr<const JEvent>& event)
{

	// Code to generate factory data goes here. Add it like:
	//
	// ExtVetofa250Converter *myExtVetofa250Converter = new ExtVetofa250Converter;
	// myExtVetofa250Converter->x = x;
	// myExtVetofa250Converter->y = y;
	// ...
	// _data.push_back(myExtVetofa250Converter);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

}

jerror_t ExtVetofa250Converter_factory::erun(void)
{

	this->clearCalibrationHandler(m_extVetofa250Converter->threshold);
	this->clearCalibrationHandler(m_extVetofa250Converter->m_pedestals);

	return NOERROR;
}

jerror_t ExtVetofa250Converter_factory::fini(void)
{
	_data.clear();
	return NOERROR;
}

