// $Id$
//
//    File: ExtVetoSummary.h
// Created: Fri Mar 11 16:16:37 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoSummary_
#define _ExtVetoSummary_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>
#include "TObject.h"

class ExtVetoSummary : public JObject, public TObject {
public:
    JOBJECT_PUBLIC(ExtVetoSummary);

    int sector;
    int nHits;
    int component;
    std::vector <TranslationTable::EXT_VETO_Index_t> hits;
    double E;
    double T;

    void Summarize(JObjectSummary& summary) const final {};

    TranslationTable::EXT_VETO_Index_t m_channel;
    oid_t ExtVetoHit_id;

};

#endif // _ExtVetoSummary_

