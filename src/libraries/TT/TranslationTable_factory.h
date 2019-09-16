// $Id$
//
//    File: TranslationTable_factory.h
// Created: Tue Jan 12 09:29:14 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _TranslationTable_factory_
#define _TranslationTable_factory_

#include "TranslationTable.h"

#include <JANA/JFactoryT.h>

class TranslationTable_factory : public JFactoryT<TranslationTable> {
public:
	TranslationTable_factory();
	~TranslationTable_factory() = default;

private:
	void Init() override;
	void ChangeRun(const std::shared_ptr<const JEvent>& aEvent) override;
	void Process(const std::shared_ptr<const JEvent>& aEvent) override;

	int isMC;
	int VERBOSE;
	TranslationTable *tt;
	string SYSTEMS_TO_PARSE;
	string ROCID_MAP_FILENAME;
	string XML_FILENAME;
	bool NO_CCDB;
};

#endif // _TranslationTable_factory_

