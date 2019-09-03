/*
 * BDXFactory.h
 *
 *  Created on: Mar 25, 2016
 *      Author: celentan
 */

#ifndef BDXFACTORY_H_
#define BDXFACTORY_H_

#include <JANA/JFactory.h>
#include <JANA/JApplication.h>
#include <JANA/JEventProcessor.h>
#include <JANA/JEventLoop.h>
#include "BDXEventProcessor.h"
#include "CalibrationHandler.h"
#include <string>

using namespace std;




template<class T> class BDXFactory : public JFactoryT<T> {
public:
	BDXFactory();
	virtual ~BDXFactory();

protected:
	void mapCalibrationHandler(CalibrationHandlerBase*);
	void updateCalibrationHandler(CalibrationHandlerBase*,JEventLoop *loop);
	void clearCalibrationHandler(CalibrationHandlerBase*);

private:
	const string BDXEventProcessor_name;
	vector<JEventProcessor*> m_processors;
	vector<JEventProcessor*>::iterator m_processors_it;
	BDXEventProcessor* 	m_BDXEventProcessor;
};

template<class T> BDXFactory<T>::BDXFactory():BDXEventProcessor_name("BDXEventProcessor"){
	m_BDXEventProcessor=0;
}

template<class T> BDXFactory<T>::~BDXFactory(){

}
template<class T> void BDXFactory<T>::mapCalibrationHandler(CalibrationHandlerBase* calib)	{

	if (m_BDXEventProcessor==0){
		m_processors=this->app->GetProcessors();
		string processor_name;
		for (m_processors_it=m_processors.begin();m_processors_it!=m_processors.end();m_processors_it++){
			processor_name=string((*m_processors_it)->className());
			if ((processor_name)==BDXEventProcessor_name){
				m_BDXEventProcessor=(BDXEventProcessor*)(*m_processors_it);
			}
		}
	}
	if (m_BDXEventProcessor){
		japp->RootWriteLock();
		m_BDXEventProcessor->addCalibration(calib);
		japp->RootUnLock();
	}
	else{
		cout<<"Error, BDXEventProcessor not found"<<endl;
	}
}

template<class T> void BDXFactory<T>::updateCalibrationHandler(CalibrationHandlerBase* calib,JEventLoop *loop){
	if (m_BDXEventProcessor==0){
			m_processors=this->app->GetProcessors();
			string processor_name;
			for (m_processors_it=m_processors.begin();m_processors_it!=m_processors.end();m_processors_it++){
				processor_name=string((*m_processors_it)->className());
				if ((processor_name)==BDXEventProcessor_name){
					m_BDXEventProcessor=(BDXEventProcessor*)(*m_processors_it);
				}
			}
		}
		if (m_BDXEventProcessor){
			japp->RootWriteLock();
			m_BDXEventProcessor->updateCalibration(calib,loop);
			japp->RootUnLock();
		}
		else{
			cout<<"Error, BDXEventProcessor not found"<<endl;
		}
}


template<class T> void BDXFactory<T>::clearCalibrationHandler(CalibrationHandlerBase* calib){
	if (m_BDXEventProcessor==0){
			m_processors=this->app->GetProcessors();
			string processor_name;
			for (m_processors_it=m_processors.begin();m_processors_it!=m_processors.end();m_processors_it++){
				processor_name=string((*m_processors_it)->className());
				if ((processor_name)==BDXEventProcessor_name){
					m_BDXEventProcessor=(BDXEventProcessor*)(*m_processors_it);
				}
			}
		}
		if (m_BDXEventProcessor){
			japp->RootWriteLock();
			m_BDXEventProcessor->clearCalibration(calib);
			japp->RootUnLock();
		}
		else{
			cout<<"Error, BDXEventProcessor not found"<<endl;
		}
}

#endif /* SRC_LIBRARIES_SYSTEM_BDXFACTORY_H_ */
