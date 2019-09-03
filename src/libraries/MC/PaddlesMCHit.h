// $Id$
//
//    File: genParticle.h
// Created: Tue Mar  1 00:59:14 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JPaddlesMCHit_
#define _JPaddlesMCHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <MC/VetoMCHit.h>

/*This class is basically a place-holder for the data coming from EVIO file,
 * for the raw and digitized banks
 */
class PaddlesMCHit : public VetoMCHit {
public:
    JOBJECT_PUBLIC(PaddlesMCHit);

    void Summarize(JObjectSummary& summary) const final {
        summary.add(id, "id", "%4d");
    }

    //dgtz banks
    int adc, tdc;

    //raw banks
    double totEdep;
};

#endif // _genParticle_

