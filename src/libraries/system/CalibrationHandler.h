// $Id$
//
//    File: CalibrationHandler.h
// Created: Tue Feb 23 17:49:00 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _CalibrationHandler_
#define _CalibrationHandler_

#include <JANA/JObject.h>
#include <TT/TranslationTable.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

enum jerror_t {
	NOERROR = 0, UNKNOWN_ERROR = -1000, MAX_EVENT_PROCESSORS_EXCEEDED, ERROR_OPENING_EVENT_SOURCE,
	ERROR_CLOSING_EVENT_SOURCE, NO_MORE_EVENTS_IN_SOURCE, NO_MORE_EVENT_SOURCES, EVENT_NOT_IN_MEMORY,
	EVENT_SOURCE_NOT_OPEN, OBJECT_NOT_AVAILABLE, DEVENT_OBJECT_DOES_NOT_EXIST, MEMORY_ALLOCATION_ERROR,
	RESOURCE_UNAVAILABLE, VALUE_OUT_OF_RANGE, INFINITE_RECURSION, UNRECOVERABLE_ERROR, FILTER_EVENT_OUT };

class CalibrationHandlerBase{
public:

	CalibrationHandlerBase(string table):m_hasLoadedCurrentRun(false){m_table=table;};
	virtual ~CalibrationHandlerBase(){};
	virtual jerror_t fillCalib(const std::vector<std::vector<double> > &calib_data)=0;

	void setLoadedCurrentRun(bool hasLoadedCurrentRun){m_hasLoadedCurrentRun=hasLoadedCurrentRun;}
	bool hasLoadedCurrentRun(){return m_hasLoadedCurrentRun;}

	string getTable(){return m_table;}
	void setTable(string table){m_table=table;}

	std::vector<std::vector<double> > getRawCalibData(){return m_rawCalibData;}

protected:
	bool m_hasLoadedCurrentRun;
	string m_table;
	std::vector<std::vector<double> > m_rawCalibData;



};


template <class T> class CalibrationHandler : public CalibrationHandlerBase{  //T is supposed to be an index in the TT library
public:
	CalibrationHandler(string name):CalibrationHandlerBase(name){};
	virtual jerror_t fillCalib(const std::vector<std::vector<double> > &calib_data);
	vector<double> getCalib(const T &index);
	std::vector < double > operator[](const T &index);

	double getCalibSingle(const T &index);



	std::map  <T, std::vector < double > > getCalibMap() const{return m_calib;}
	std::map  <T, std::vector < double > > getActualCalibMap() const{return m_actualCalib;}



	//	virtual ~CalibrationHandler();
private:

	std::map <T, std::vector < double > > m_calib;
	std::vector < std::vector < double > > m_actualCalib;
	std::vector < T > m_actualCalibIndex;
	std::pair<typename std::map <T, std::vector < double > >::iterator,bool> m_insert_check;


};

template <class T> jerror_t CalibrationHandler<T>::fillCalib(const std::vector<std::vector<double> > &calib_data){
	T index;
	vector<double> data;
	int nData=0;
	int prevNdata=0;
	m_calib.clear();

	m_rawCalibData.clear();
	m_rawCalibData=calib_data;

	for (int irow=0;irow<calib_data.size();irow++){
		data.clear();
		if (irow>0) prevNdata=nData;
		nData=calib_data[irow].size()-index.nIDs();
		if (nData<=0) {
			jerr<<"Error in CalibrationHandler<T>:::fillCalib. No data?"<<jendl;
			m_calib.clear();
			return 	VALUE_OUT_OF_RANGE;
		}
		if ((nData!=prevNdata)&&(irow!=0)){
			jerr<<"Error in CalibrationHandler<T>::fillCalib. Error on number of datas?"<<jendl;
			m_calib.clear();
			return 	VALUE_OUT_OF_RANGE;
		}
		/*Fill the indexes*/
		for (int iindex=0;iindex<index.nIDs();iindex++){
			index.ID(iindex)=calib_data[irow][iindex];
		}
		/*Fill the data*/
		for (int idata=0;idata<nData;idata++){
			data.push_back(calib_data[irow][index.nIDs()+idata]);
		}
		m_insert_check=m_calib.insert(std::make_pair(index,data));
		if (m_insert_check.second==false){
			jout<<"Element already exists in the map "<<m_table<<" Doing nothing"<<jendl;
			jout<<"Index print: "<<index.print()<<jendl;
		}
	}
	return NOERROR;
}

template <class T> std::vector <double> CalibrationHandler<T>::operator[](const T &index){
	return this->getCalib(index);
}

template <class T> vector<double> CalibrationHandler<T>::getCalib(const T &index){

	vector<double> ret(0);
	int pos;
	typename std::vector < T >::iterator vit;
	typename std::map<T , vector < double > >::iterator it;



	vit = find(m_actualCalibIndex.begin(),m_actualCalibIndex.end(),index);


	if (vit!=m_actualCalibIndex.end()){
		pos=vit - m_actualCalibIndex.begin();
		return m_actualCalib[pos];
	}
	else{
		it=m_calib.find(index);
		if (it==m_calib.end()){
			jerr<<"CalibrationHandler<T>::getCalib element not found"<<jendl;
			jerr<<"T: "<<index.name()<<jendl;
			jerr<<index.print()<<jendl;
			return ret;
		}
		else{
			m_actualCalibIndex.push_back(index);
			m_actualCalib.push_back(it->second);
			return it->second;
		}
	}

}

template <class T> double CalibrationHandler<T>::getCalibSingle(const T &index){
	vector<double> this_data;
	this_data=getCalib(index);
	if (this_data.size()==1){
		return this_data.at(0);
	}
	else{
		jerr<<"CalibrationHandler<T>::getCalibSingle error: different than 1 entry: "<<this_data.size()<<jendl;
		jerr<<"Table: "<<m_table<<jendl;
		jerr<<"T: "<<index.name()<<jendl;
		jerr<<"T print: "<<index.print()<<jendl;
		return 0;
	}
}


#endif // _CalibrationHandler_

