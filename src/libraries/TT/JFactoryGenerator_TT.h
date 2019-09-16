// $Id$
//
//    File: JFactoryGenerator_TranslationTable.h
// Created: Tue Jan 12 09:28:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_TranslationTable_
#define _JFactoryGenerator_TranslationTable_

#include "TranslationTable_factory.h"

#include <JANA/JFactoryGenerator.h>


class JFactoryGenerator_TT : public JFactoryGenerator {

public:
    JFactoryGenerator_TT() = default;

    virtual ~JFactoryGenerator_TT() = default;

    void GenerateFactories(JFactorySet* factory_set) override {
        factory_set->Add(new TranslationTable_factory());
    }

};

#endif // _JFactoryGenerator_TranslationTable_

