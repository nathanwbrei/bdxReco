// $Id$
//
//    File: Calorimeterfa250Converter_factory.cc
// Created: Mon Jan 25 17:02:27 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>

#include "TString.h"
using namespace std;

#include "Calorimeterfa250Converter_factory.h"


int PThreadIDUniqueInt(pthread_t tid){
	int ret;
	memcpy(&ret, &tid, std::min(sizeof(tid), sizeof(ret)));
	return ret;
}

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
jerror_t Calorimeterfa250Converter_factory::init(void)
{


	m_thr=new CalibrationHandler<TranslationTable::CALO_Index_t>("/Calorimeter/thr");
	this->mapCalibrationHandler(m_thr);
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t Calorimeterfa250Converter_factory::brun(JEventLoop *eventLoop, int32_t runnumber)
{

	this->updateCalibrationHandler(m_thr,eventLoop);
	int threadId= PThreadIDUniqueInt(eventLoop->GetPThreadID());
	m_calorimeterfa250Converter=new Calorimeterfa250Converter();
	m_calorimeterfa250Converter->name()=string(Form("h%i",threadId));
	japp->GetParameter("CALORIMETER:VERBOSE",	m_calorimeterfa250Converter->verbose());

	m_calorimeterfa250Converter->m_NSB=m_NSB;
	m_calorimeterfa250Converter->m_NSA=m_NSA;
	m_calorimeterfa250Converter->m_NPED=m_NPED;
	m_calorimeterfa250Converter->m_NSAMPLES=m_NSAMPLES;
	m_calorimeterfa250Converter->m_thrDB=m_thr;


	_data.push_back(m_calorimeterfa250Converter);
	SetFactoryFlag(PERSISTANT);

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t Calorimeterfa250Converter_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{



	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t Calorimeterfa250Converter_factory::erun(void)
{
	//if (m_calorimeterfa250Converter!=0) delete m_calorimeterfa250Converter;
	_data.clear();
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t Calorimeterfa250Converter_factory::fini(void)
{
	/*if (m_calorimeterfa250Converter!=0){
		delete m_calorimeterfa250Converter;
	}*/
	_data.clear();
	return NOERROR;
}

