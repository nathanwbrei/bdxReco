// $Id$
//
//    File: Calorimeterfa250Converter_factory.h
// Created: Mon Jan 25 17:02:27 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _Calorimeterfa250Converter_factory_
#define _Calorimeterfa250Converter_factory_

#include <JANA/JFactoryT.h>
#include "Calorimeterfa250Converter.h"

class Calorimeterfa250Converter_factory:public JFactoryT<Calorimeterfa250Converter>{
	public:
		Calorimeterfa250Converter_factory();
		~Calorimeterfa250Converter_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();

		//int m_isFirstCallToBrun;
		Calorimeterfa250Converter *m_calorimeterfa250Converter;

		int m_NSB;
		int m_NSA;
		int m_NPED;
		int m_NSAMPLES;

		CalibrationHandler<TranslationTable::CALO_Index_t> *m_thr;

};

#endif // _Calorimeterfa250Converter_factory_

