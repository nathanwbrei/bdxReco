// $Id$
//
//    File: fa250Mode1CalibPedSubHitFFT_factory.h
// Created: Mon May 27 15:32:26 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#ifndef _fa250Mode1CalibPedSubHitFFT_factory_
#define _fa250Mode1CalibPedSubHitFFT_factory_

#include "fa250Mode1CalibPedSubHitFFT.h"
#include <JANA/JFactoryT.h>
#include <JANA/JEvent.h>

class fa250Mode1CalibPedSubHitFFT_factory : public JFactoryT<fa250Mode1CalibPedSubHitFFT> {

public:
    fa250Mode1CalibPedSubHitFFT_factory() = default;
    ~fa250Mode1CalibPedSubHitFFT_factory() override = default;

private:
    void Init() override {};
    void ChangeRun(const std::shared_ptr<const JEvent>& aEvent) override {}
    void Process(const std::shared_ptr<const JEvent>& aEvent) override;
};

#endif // _fa250Mode1CalibPedSubHitFFT_factory_

