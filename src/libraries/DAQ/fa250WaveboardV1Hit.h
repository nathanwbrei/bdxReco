// $Id$
//
//    File: fa250Mode1Hit.h
// Created: Mon Dec 28 11:04:04 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _fa250WaveboardV1Hit_
#define _fa250WaveboardV1Hit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "fa250Hit.h"


class TH1D;
class TCanvas;

class fa250WaveboardV1Hit : public fa250Hit {
public:
    JOBJECT_PUBLIC(fa250WaveboardV1Hit);

    fa250WaveboardV1Hit();

    virtual ~fa250WaveboardV1Hit();

    vector<double> samples;
    int chargeFirstDBnopedsub;

    void Summarize(JObjectSummary& summary) const final {
        summary.add(m_channel.rocid, "crate", "%4d");
        summary.add(m_channel.slot, "slot", "%4d");
        summary.add(m_channel.channel, "channel", "%4d");
        summary.add((long long) timestamp, "timestamp", "%lld");
        summary.add(samples.size(), "nsamples", "%4d");
        summary.add(chargeFirstDBnopedsub, "charge(firstDF,noPedSub)", "%4d");
    }

    void toHisto(TH1D* h) const;

    virtual TCanvas* Draw(int id = 0) const;

protected:
    mutable TH1D* hWave;
};

#endif // _fa250Mode1Hit_

