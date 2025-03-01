// $Id$
//
//    File: ExtVetoDigiHit_factory.cc
// Created: Wed Jan 13 21:06:40 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

//objects we need from the framework
#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode7Hit.h>
#include <TT/TranslationTable.h>
//objects we put in the framework
#include <ExtVeto/ExtVetoPMTHit.h>
#include <ExtVeto/ExtVetofa250Converter.h>

#include "ExtVetoPMTHit_factory.h"
#include <JANA/JEvent.h>

//------------------
// init
//------------------
void ExtVetoPMTHit_factory::Init()
{
	m_PMT_gain=new CalibrationHandler<TranslationTable::EXT_VETO_Index_t>("/ExtVeto/PMT_gain");
	m_calibration_service = japp->GetService<BDXCalibrationService>();
	m_calibration_service->addCalibration(m_PMT_gain);
}

//------------------
// brun
//------------------
void ExtVetoPMTHit_factory::ChangeRun(const std::shared_ptr<const JEvent>& event)
{
	jout<<"ExtVetoDigiHit_factory::brun new run number: "<<event->GetRunNumber()<<jendl;
	m_tt=0;
	event->Get(&m_tt);
	if (m_tt==0){
		jerr<<" unable to get the TranslationTable from this run!"<<jendl;
		throw JException(" unable to get the TranslationTable from this run!");
	}

	m_extVetofa250Converter=0;
	event->Get(&m_extVetofa250Converter);
	if (m_extVetofa250Converter==0){
		jerr<<" unable to get the extVetofa250Converter!"<<jendl;
		throw JException(" unable to get the extVetofa250Converter!");
	}

	m_calibration_service->updateCalibration(m_PMT_gain, event->GetRunNumber(), event->GetEventNumber());

	japp->GetParameter("EXTVETO:VERBOSE",VERBOSE);

	if (VERBOSE>3){
		std::map  < TranslationTable::EXT_VETO_Index_t, std::vector < double > > gainCalibMap;
		std::map  < TranslationTable::EXT_VETO_Index_t, std::vector < double > >::iterator gainCalibMap_it;
		gainCalibMap=m_PMT_gain->getCalibMap();
		jout<<"Got following PMT_gain for run number: "<<event->GetRunNumber()<<jendl;
		jout<<"Rows: "<<gainCalibMap.size()<<jendl;
		for (gainCalibMap_it=gainCalibMap.begin();gainCalibMap_it!=gainCalibMap.end();gainCalibMap_it++){
			jout<<gainCalibMap_it->first.sector<<" "<<gainCalibMap_it->first.layer<<" "<<gainCalibMap_it->first.component<<" "<<gainCalibMap_it->first.readout<<" "<<gainCalibMap_it->second.at(0)<<jendl;

		}
	}
}

//------------------
// evnt
//------------------
void ExtVetoPMTHit_factory::Process(const std::shared_ptr<const JEvent>& event)
{
	TranslationTable::ChannelInfo m_channel;
	TranslationTable::csc_t		  m_csc;
	double		 				  m_q_calib;
	ExtVetoPMTHit *m_ExtVetoPMTHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const fa250Mode1Hit*> m_fa250Mode1Hit;
	vector <const fa250Mode7Hit*> m_fa250Mode7Hit;
	vector <const fa250Mode1Hit*>::const_iterator it_fa250Mode1Hit;
	vector <const fa250Mode7Hit*>::const_iterator it_fa250Mode7Hit;

	//1b: retrieve objects
	event->Get(m_fa250Mode1Hit);
	event->Get(m_fa250Mode7Hit);
	//	jout << "sono qui" <<std::endl;
	/*2: Now we have the daq objects, still indexed as "crate-slot-channel"
	 *	 Use the translation table to produce the digitized hit of the inner veto
	 *	 Note that we can produce a single object type here, i.e. ExtVetoPMTHit,
	 *	 but we have 2 possible source, mode 1 and mode 7.
	 *	 Therefore, we will use the TranslationTable ONLY to check if this crate-slot-channel
	 *	 combination refers to a InnerVeto hit and, in case, to determine which one, i.e. which ID in the InnerVeto scheme.
	 *	 Then, we will proceed in two different ways.
	 */
	/*First, mode 1*/
	/*Note that in this case we have to integrate the pulse - it is a mode 1 pulse! */
	for (it_fa250Mode1Hit=m_fa250Mode1Hit.begin();it_fa250Mode1Hit!=m_fa250Mode1Hit.end();it_fa250Mode1Hit++){
		m_channel=m_tt->getChannelInfo((*it_fa250Mode1Hit)->m_channel);

		if (m_channel.det_sys==TranslationTable::EXT_VETO){
			//A.C. do not touch these
			m_ExtVetoPMTHit=m_extVetofa250Converter->convertHit((fa250Hit*)*it_fa250Mode1Hit,m_channel);
			m_ExtVetoPMTHit->AddAssociatedObject(*it_fa250Mode1Hit);

			//Apply phe conversion
			m_q_calib=m_PMT_gain->getCalibSingle(*(m_channel.ext_veto));
			if (m_q_calib>0){
				m_ExtVetoPMTHit->Q/=((1.602*1E-19)*1E9);	// number of electrons at the exit of the PMT
				m_ExtVetoPMTHit->Q/=m_q_calib;		// number of phe
			}


			mData.push_back(m_ExtVetoPMTHit);
		}
	}



	/*Then, mode 7*/
	/*Note that in this case we do not have to integrate the pulse - it is a mode 7 pulse! */
	for (it_fa250Mode7Hit=m_fa250Mode7Hit.begin();it_fa250Mode7Hit!=m_fa250Mode7Hit.end();it_fa250Mode7Hit++){


		m_channel=m_tt->getChannelInfo((*it_fa250Mode7Hit)->m_channel);
		if (m_channel.det_sys==TranslationTable::EXT_VETO){
			//A.C. do not touch these
			m_ExtVetoPMTHit=m_extVetofa250Converter->convertHit((fa250Hit*)*it_fa250Mode7Hit,m_channel);
			m_ExtVetoPMTHit->AddAssociatedObject(*it_fa250Mode7Hit);
			/*Apply phe conversion */
			m_q_calib=m_PMT_gain->getCalibSingle(*(m_channel.ext_veto));
			if (m_q_calib>0){
				m_ExtVetoPMTHit->Q/=((1.602*1E-19)*1E9);	// number of electrons at the exit of the PMT
				m_ExtVetoPMTHit->Q/=m_q_calib;		// number of phe
			}



			mData.push_back(m_ExtVetoPMTHit);
		}
	}
}

//------------------
// erun
//------------------
void ExtVetoPMTHit_factory::EndRun()
{
	m_calibration_service->clearCalibration(m_PMT_gain);
}

//------------------
// fini
//------------------
void ExtVetoPMTHit_factory::Finish()
{
}

