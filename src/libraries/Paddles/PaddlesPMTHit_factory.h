// $Id$
//
//    File: ExtVetoDigiHit_factory.h
// Created: Wed Jan 13 21:06:40 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _PaddlesPMTHit_factory_
#define _PaddlesPMTHit_factory_

#include <JANA/JFactoryT.h>
#include <Paddles/PaddlesPMTHit.h>
#include <system/CalibrationHandler.h>

class TranslationTable;
class Paddlesfa250Converter;



class PaddlesPMTHit_factory:public JFactoryT<PaddlesPMTHit>{
	public:
		PaddlesPMTHit_factory():m_tt(0),m_Paddlesfa250Converter(0),m_PMT_gain(0){};
		~PaddlesPMTHit_factory(){};


	private:
		void Init() override;
		void ChangeRun(const std::shared_ptr<const JEvent>& event) override;
		void Process(const std::shared_ptr<const JEvent>& aEvent) override;
		void EndRun();
		void Finish();

		const TranslationTable *m_tt;
		const Paddlesfa250Converter *m_Paddlesfa250Converter;

		CalibrationHandler<TranslationTable::PADDLES_Index_t> *m_PMT_gain;

		int VERBOSE;

};

#endif // _PaddlesPMTHit_factory_

