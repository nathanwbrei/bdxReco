// $Id$
//
//    File: Calorimeterfa250Converter_factory.cc
// Created: Mon Jan 25 17:02:27 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
#include <JANA/JEvent.h>
#include <JANA/Utils/JCpuInfo.h>

#include "TString.h"
using namespace std;

#include "Calorimeterfa250Converter_factory.h"


Calorimeterfa250Converter_factory::Calorimeterfa250Converter_factory():m_calorimeterfa250Converter(0){

	m_NPED=20;
	m_NSAMPLES=2000;
	m_thr=0;

	japp->SetDefaultParameter("CALORIMETER:NSB",m_NSB,"Samples before the maximum to integrate for single phes - in ns");
	japp->SetDefaultParameter("CALORIMETER:NSA",m_NSA,"Samples after the maximum to integrate for single phes - in ns");
	japp->SetDefaultParameter("CALORIMETER:NPED",m_NPED,"Number of samples to include in the pedestal computation event-by-event - in ns");
	japp->SetDefaultParameter("CALORIMETER:NSAMPLES",m_NSAMPLES,"Number of samples to integrate at maximum for good signals - in ns");


}
//------------------
// init
//------------------
void Calorimeterfa250Converter_factory::Init()
{
	m_thr=new CalibrationHandler<TranslationTable::CALO_Index_t>("/Calorimeter/thr");
	m_calibration_service = japp->GetService<BDXCalibrationService>();
	m_calibration_service->addCalibration(m_thr);
}

//------------------
// brun
//------------------
void Calorimeterfa250Converter_factory::ChangeRun(const std::shared_ptr<const JEvent>& event)
{

    m_calibration_service->updateCalibration(m_thr, event->GetRunNumber(), event->GetEventNumber());
	int threadId = JCpuInfo::GetCpuID();
	m_calorimeterfa250Converter=new Calorimeterfa250Converter();
	m_calorimeterfa250Converter->name()=string(Form("h%i",threadId));
	japp->GetParameter("CALORIMETER:VERBOSE", m_calorimeterfa250Converter->verbose());

	m_calorimeterfa250Converter->m_NSB=m_NSB;
	m_calorimeterfa250Converter->m_NSA=m_NSA;
	m_calorimeterfa250Converter->m_NPED=m_NPED;
	m_calorimeterfa250Converter->m_NSAMPLES=m_NSAMPLES;
	m_calorimeterfa250Converter->m_thrDB=m_thr;

	Insert(m_calorimeterfa250Converter);
	SetFactoryFlag(PERSISTANT);
}

//------------------
// evnt
//------------------
void Calorimeterfa250Converter_factory::Process(const std::shared_ptr<const JEvent>& event)
{
}

//------------------
// erun
//------------------
void Calorimeterfa250Converter_factory::EndRun()
{
	//if (m_calorimeterfa250Converter!=0) delete m_calorimeterfa250Converter;
	mData.clear();
}

//------------------
// fini
//------------------
void Calorimeterfa250Converter_factory::Finish()
{
	/*if (m_calorimeterfa250Converter!=0){
		delete m_calorimeterfa250Converter;
	}*/
	mData.clear();
}

