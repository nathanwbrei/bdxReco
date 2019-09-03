// $Id$
//
//    File: FadcConverter.h
// Created: Wed Jan 20 19:27:33 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _FadcConverter_
#define _FadcConverter_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include <DAQ/fa250Hit.h>
#include <TT/TranslationTable.h>
#include "TObject.h"

#include <cmath>

template<class T> class fa250Converter : public JObject, public TObject {
	public:
		JOBJECT_PUBLIC(fa250Converter);
		

		virtual T* convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel,int eventN=0)const=0; //also this one is purely virtual
		
		/*Add here some "common" member functions*/
		double sumSamples(int N,const double *samples) const;
		double sumSamples(int first,int last,const double *samples) const;

		double getMaximum(int N,const double *x,double &Tmax) const;
		double getMaximum(int first,int last,const double *x,double &Tmax) const;

		double getRMS(int N,double *x) const;
		double getRMS(int first,int last,double *x) const;
		int& verbose(){return VERBOSE;}
		string& name(){return mName;}

	protected:
		int VERBOSE;
		std::string mName; //just an unique string in this tread

};




template<class T> double fa250Converter< T >::sumSamples(int first,int last,const double *samples) const{
	double ret=0;
	for (int ii=first;ii<=last;ii++) ret+=samples[ii];
	return ret;
}

template<class T> double fa250Converter< T >::sumSamples(int N,const double *samples) const{
	return this->sumSamples(0,N-1,samples);
}
template<class T> double fa250Converter< T >::getMaximum(int first,int last,const double *x,double &Tmax) const{
		double max=x[first];
		Tmax=first;
		for (int ii=first;ii<=last;ii++){
			if (x[ii]>max){
				Tmax=ii;
				max=x[ii];
			}
		}
		return max;
}


template<class T> double fa250Converter< T >::getMaximum(int N,const double *x,double &Tmax) const{
	return this->getMaximum(0,N-1,x,Tmax);
}

template<class T> double fa250Converter< T >::getRMS(int first,int last,double *x) const{
	double rms=0;
	double mean=this->sumSamples(first,last,x);
	mean/=(last-first+1);

	for (int ii=first;ii<=last;ii++) rms+=x[ii]*x[ii];

	rms=rms-mean*mean;
	rms=sqrt(rms);
	return rms;


}
template<class T> double fa250Converter< T >::getRMS(int N,double *x) const{
	return this->getRMS(0,N-1,x);
}


#endif // _FadcConverter_

