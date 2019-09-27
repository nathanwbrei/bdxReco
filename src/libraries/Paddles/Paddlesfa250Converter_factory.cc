// $Id$
//
//    File: ExtVetofa250Converter_factory.cc
// Created: Mon Jan 25 16:28:59 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include <JANA/JEvent.h>
#include <Paddles/Paddlesfa250Converter_factory.h>
#include <system/CalibrationHandler.h>

//------------------
// init
//------------------
void Paddlesfa250Converter_factory::Init()
{
	m_isFirstCallToBrun=1;
	m_calibration_service = japp->GetService<BDXCalibrationService>();
	m_Paddlesfa250Converter=new Paddlesfa250Converter();

	m_Paddlesfa250Converter->threshold=new CalibrationHandler<TranslationTable::PADDLES_Index_t>("/Paddles/Threshold");
	m_calibration_service->addCalibration(m_Paddlesfa250Converter->threshold);

	m_Paddlesfa250Converter->m_pedestals=new DAQCalibrationHandler("/DAQ/pedestals");
	m_calibration_service->addCalibration(m_Paddlesfa250Converter->m_pedestals);
}

//------------------
// brun
//------------------
void Paddlesfa250Converter_factory::ChangeRun(const std::shared_ptr<const JEvent>& event)
{

	m_calibration_service->updateCalibration(m_Paddlesfa250Converter->threshold, event->GetRunNumber(), event->GetEventNumber());
	m_calibration_service->updateCalibration(m_Paddlesfa250Converter->m_pedestals, event->GetRunNumber(), event->GetEventNumber());

	if (m_isFirstCallToBrun){
			m_isFirstCallToBrun=0;
			mData.push_back(m_Paddlesfa250Converter);
			SetFactoryFlag(PERSISTANT);
		}
}

//------------------
// evnt
//------------------
void Paddlesfa250Converter_factory::Process(const std::shared_ptr<const JEvent>& event)
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

//------------------
// erun
//------------------
void Paddlesfa250Converter_factory::EndRun()
{

	m_calibration_service->clearCalibration(m_Paddlesfa250Converter->threshold);
	m_calibration_service->clearCalibration(m_Paddlesfa250Converter->m_pedestals);
}

//------------------
// fini
//------------------
void Paddlesfa250Converter_factory::Finish()
{
    mData.clear(); // TODO: Why is this here? Unless there is a good reason, delete this
}

