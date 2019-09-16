// $Id$
//
//    File: fa250Mode1PedSubHit_factory.h
// Created: Wed Feb  3 07:38:15 EST 2016
// Creator: davidl (on Darwin harriet.jlab.org 13.4.0 i386)
//

#ifndef _fa250Mode1PedSubHit_factory_
#define _fa250Mode1PedSubHit_factory_

#include <JANA/JFactory.h>
#include <system/BDXFactory.h>
#include <DAQ/DAQCalibrationHandler.h>
#include "fa250Mode1CalibPedSubHit.h"
#include "fa250WaveboardV1Hit.h"

class fa250Mode1CalibPedSubHit_factory : public BDXFactory<fa250Mode1CalibPedSubHit> {
public:
    fa250Mode1CalibPedSubHit_factory() : m_pedestals(nullptr) {};
    ~fa250Mode1CalibPedSubHit_factory() override = default;


private:
    void Init() override;
    void ChangeRun(const std::shared_ptr<const JEvent>& aEvent) override;
    void Process(const std::shared_ptr<const JEvent>& aEvent) override;

    DAQCalibrationHandler* m_pedestals;
    DAQCalibrationHandler* m_parms;
    double LSB; //LSB in mV
    double dT;  //sampling time
};

#endif // _fa250Mode1PedSubHit_factory_

