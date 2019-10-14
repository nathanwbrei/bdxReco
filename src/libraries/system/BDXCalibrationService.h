//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Jefferson Science Associates LLC Copyright Notice:
//
// Copyright 251 2014 Jefferson Science Associates LLC All Rights Reserved. Redistribution
// and use in source and binary forms, with or without modification, are permitted as a
// licensed user provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this
//    list of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products derived
//    from this software without specific prior written permission.
// This material resulted from work developed under a United States Government Contract.
// The Government retains a paid-up, nonexclusive, irrevocable worldwide license in such
// copyrighted data to reproduce, distribute copies to the public, prepare derivative works,
// perform publicly and display publicly and to permit others to do so.
// THIS SOFTWARE IS PROVIDED BY JEFFERSON SCIENCE ASSOCIATES LLC "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
// JEFFERSON SCIENCE ASSOCIATES, LLC OR THE U.S. GOVERNMENT BE LIABLE TO LICENSEE OR ANY
// THIRD PARTES FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
// Author: Nathan Brei
//

#ifndef BDXRECO_BDXSERVICE_H
#define BDXRECO_BDXSERVICE_H

#include <JANA/JApplication.h>
#include <JANA/Services/JServiceLocator.h>
#include <JANA/Services/JGlobalRootLock.h>
#include <JANA/Services/JCalibrationManager.h>

#include "CalibrationHandler.h"
#include "TFile.h"

/// BDXService provides a single access point for calibration data and ROOT locks.
/// It is intended as a replacement for BDXEventProcessor and BDXFactory.
class BDXCalibrationService : public JService {

    std::shared_ptr<JGlobalRootLock> m_root_lock;
    std::shared_ptr<JCalibrationManager> m_calibration_manager;

    /* Important: We can't have CalibrationHandlers in each factory,
     * since this requires access to calibrations in each thread. And this may be time consuming,
     * specially if using remote mysql.
     * Therefore,
     * - Have the factories creating their own calibrations
     * - Have them adding these to the m_calibrations vector, containing a pair of the pointer and a string to the CCDB path
     * - Have factories getting back the pointer in their brun method
     */
    map<string,vector<CalibrationHandlerBase*> >  m_calibrations;
    map<string,vector<CalibrationHandlerBase*> >::iterator m_calibrations_it;

    JLogger bout;
    JLogger berr;

public:
    void acquire_services(JServiceLocator* sl) override {
        m_root_lock = sl->get<JGlobalRootLock>();
        m_calibration_manager = sl->get<JCalibrationManager>();
    };

    void addCalibration(CalibrationHandlerBase* cal) {
        m_root_lock->acquire_write_lock();
        this->m_calibrations[cal->getTable()].push_back(cal);
        m_root_lock->release_lock();
    }

    void updateCalibration(CalibrationHandlerBase* cal, unsigned int run_number, unsigned long int event_number) {
        m_root_lock->acquire_write_lock();
        /*First, check the table name for cal*/
        string name = cal->getTable();
        /*Verify the key is in the map*/
        m_calibrations_it = m_calibrations.find(name);
        if (m_calibrations_it == m_calibrations.end()) {
            LOG << "BDXEventProcessor::updateCalibration error, calibration handler associated with table " << name << " was not registered!" << LOG_END;
            return;
        }

        vector<CalibrationHandlerBase*> calibrations = m_calibrations[name];
        vector<CalibrationHandlerBase*>::iterator calibrations_it;

        /*Another check*/
        calibrations_it = find(calibrations.begin(), calibrations.end(), cal);
        if (calibrations_it == calibrations.end()) {
            LOG << "UpdateCalibration, key: " << name << " was found but not this specific calibrationHandler!" << LOG_END;
            return;
        }
        /*Verify if all the calibrators already have been set for this run*/
        /*Since we are multi-thread mode, it is possible this method is called by different factories at different times.
         * Need to perform a clever check:
         * 1) Loop over all the calibrations handlers for the given table.
         * 1A) If ALL off them have been loaded, do nothing
         * 1B) If NONE of them have been loaded, do that
         * 1C) If ONE of them at least have been loaded, use it for all the non-calibrated ones
         */
        bool flagAll = true;
        int calibratedOne = -1;
        for (calibrations_it = calibrations.begin(); calibrations_it != calibrations.end(); calibrations_it++) {
            if (!(*calibrations_it)->hasLoadedCurrentRun()) flagAll = false;
            else
                calibratedOne = std::distance(calibrations.begin(), calibrations_it); //save the index of this calibrated object
        }
        if (flagAll) { /*flagAll is true if ALL off them have been loaded*/
            LOG << "Going to fill CalibrationHandlers for table: " << name << " there are: " << calibrations.size() << " ALREADY DONE " << LOG_END;
            return;
        } else if (calibratedOne != -1) { /*It means there is at least an already-calibrated object!*/
            LOG << "Going to fill CalibrationHandlers for table: " << name << " there are: " << calibrations.size() << " Load from data: " << calibratedOne << LOG_END;
            for (int ical = 0; ical < calibrations.size(); ical++) {
                if (ical == calibratedOne) continue;
                else if (calibrations[ical]->hasLoadedCurrentRun()) continue;
                else {
                    calibrations[ical]->fillCalib(calibrations[calibratedOne]->getRawCalibData());
                }
            }
        } else {
            /*Get the data*/
            vector<vector<double> > m_data;
            m_calibration_manager->GetCalib(run_number, event_number, name, m_data);
            LOG << "Going to fill CalibrationHandlers for table: " << name << " there are: " << calibrations.size() << " Load from DB " << LOG_END;
            for (calibrations_it = calibrations.begin(); calibrations_it != calibrations.end(); calibrations_it++) {
                (*calibrations_it)->fillCalib(m_data);
                (*calibrations_it)->setLoadedCurrentRun(true);
            }
        }
        LOG << "Done table" << name << LOG_END;
        m_root_lock->release_lock();

    }

    void clearCalibration(CalibrationHandlerBase* cal) {

        m_root_lock->acquire_write_lock();
        /*First, check the table name for cal*/
        string name = cal->getTable();
        /*Verify the key is in the map*/
        m_calibrations_it = m_calibrations.find(name);
        if (m_calibrations_it == m_calibrations.end()) {
            LOG << "BDXEventProcessor::clearCalibration error, calibration handler associated with table " << name << " was not registered!" << LOG_END;
            return;
        }

        vector<CalibrationHandlerBase*> calibrations = m_calibrations[name];
        vector<CalibrationHandlerBase*>::iterator calibrations_it;

        /*Another check*/
        calibrations_it = find(calibrations.begin(), calibrations.end(), cal);
        if (calibrations_it == calibrations.end()) {
            LOG << "BDXEventProcessor::clearCalibration, key: " << name << " was found but not this specific calibrationHandler!" << LOG_END;
            return;
        }
        for (calibrations_it = calibrations.begin(); calibrations_it != calibrations.end(); calibrations_it++) {
            (*calibrations_it)->setLoadedCurrentRun(false);
        }
        m_root_lock->release_lock();
    }

};

#endif //BDXRECO_BDXSERVICE_H
