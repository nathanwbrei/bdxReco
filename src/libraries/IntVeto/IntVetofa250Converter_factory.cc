// $Id$
//
//    File: IntVetofa250Converter_factory.cc
// Created: Mon Jan 25 17:02:27 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "IntVetofa250Converter_factory.h"
#include <JANA/JEvent.h>

IntVetofa250Converter_factory::IntVetofa250Converter_factory():
				m_isFirstCallToBrun(1),m_intVetofa250Converter(0){
	jout<<"IntVetofa250Converter_factory::creator"<<jendl;


	m_minTot=12;
	m_thr=1.5;
	m_NSB=20;
	m_NSA=30;
	m_NPED=10;
	m_RMSTHRscale=5;

	japp->SetDefaultParameter("INTVETO:MIN_TOT",m_minTot,"Min ToT (in ns) for a pulse to be considered");
	japp->SetDefaultParameter("INTVETO:THR",m_thr,"Min amplitude (in number of phe) for a pulse to be considered");
	japp->SetDefaultParameter("INTVETO:NSB",m_NSB,"Samples before the maximum (in ns) to include in integration");
	japp->SetDefaultParameter("INTVETO:NSA",m_NSA,"Samples after the maximum  (in ns) to include in integration");
	japp->SetDefaultParameter("INTVETO:NPED",m_NPED,"Number of samples (in ns) in pedestal evaluation");
	japp->SetDefaultParameter("INTVETO:RMSSCALE",m_RMSTHRscale,"Scale factor for RMS in ped. subtraction");
}

//------------------
// init
//------------------
void IntVetofa250Converter_factory::Init()
{
	jout<<"IntVetofa250Converter_factory::init"<<jendl;
	m_intVetofa250Converter=new IntVetofa250Converter();
	m_intVetofa250Converter->m_thrCalib=new CalibrationHandler<TranslationTable::INT_VETO_Index_t>("/InnerVeto/sipm_ampl");
	m_calibration_service = japp->GetService<BDXCalibrationService>();
	m_calibration_service->addCalibration(m_intVetofa250Converter->m_thrCalib);
	jout<<"IntVetofa250Converter_factory::init done"<<jendl;
}

//------------------
// brun
//------------------
void IntVetofa250Converter_factory::ChangeRun(const std::shared_ptr<const JEvent>& event)
{
	jout<<"IntVetofa250Converter_factory::brun"<<jendl;
	m_calibration_service->updateCalibration(m_intVetofa250Converter->m_thrCalib, event->GetRunNumber(), event->GetEventNumber());

	japp->GetParameter("INTVETO:VERBOSE",m_intVetofa250Converter->verbose());

	m_intVetofa250Converter->m_minTot=m_minTot;
	m_intVetofa250Converter->m_thr=m_thr;
	m_intVetofa250Converter->m_NSB=m_NSB;
	m_intVetofa250Converter->m_NSA=m_NSA;
	m_intVetofa250Converter->m_NPED=m_NPED;

	m_intVetofa250Converter->m_RMSTHRscale=m_RMSTHRscale;

	if (m_isFirstCallToBrun){
		mData.push_back(m_intVetofa250Converter);
		m_isFirstCallToBrun=0;
	}
	SetFactoryFlag(PERSISTANT);
}

//------------------
// evnt
//------------------
void IntVetofa250Converter_factory::Process(const std::shared_ptr<const JEvent>& event)
{

	// Code to generate factory data goes here. Add it like:
	//
	// IntVetofa250Converter *myIntVetofa250Converter = new IntVetofa250Converter;
	// myIntVetofa250Converter->x = x;
	// myIntVetofa250Converter->y = y;
	// ...
	// _data.push_back(myIntVetofa250Converter);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

}

//------------------
// erun
//------------------
void IntVetofa250Converter_factory::EndRun()
{

	m_calibration_service->clearCalibration(m_intVetofa250Converter->m_thrCalib);
	/*if (m_intVetofa250Converter!=0){
		delete m_intVetofa250Converter;
	}*/
	mData.clear();
}

//------------------
// fini
//------------------
void IntVetofa250Converter_factory::Finish()
{
/*	if (m_intVetofa250Converter!=0){
		delete m_intVetofa250Converter;
	}*/
	mData.clear();
}

