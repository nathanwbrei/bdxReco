// bdx reconstruction software

// JANA headers
#include <JANA/JApplication.h>
#include <JANA/Services/JGlobalRootLock.h>

// bdx headers

#include <system/JEventSourceEVIOGenerator.h>
#include <system/JEventSourceTRIDASGenerator.h>
#include <system/BDXEventProcessor.h>

//factory generators
#include <system/JFactoryGenerator_system.h>
#include <DAQ/JFactoryGenerator_DAQ.h>
#include <EPICS/JFactoryGenerator_EPICS.h>
#include <TT/JFactoryGenerator_TT.h>
#include <IntVeto/JFactoryGenerator_IntVeto.h>
#include <ExtVeto/JFactoryGenerator_ExtVeto.h>
#include <Calorimeter/JFactoryGenerator_Calorimeter.h>
#include <Paddles/JFactoryGenerator_Paddles.h>
#include <BDXmini/JFactoryGenerator_BDXmini.h>
#include <EventBuilder/JFactoryGenerator_EventBuilder.h>

#ifdef MC_SUPPORT_ENABLE
#include <MC/JFactoryGenerator_MC.h>
#endif


#include "TH1D.h"
// C++ headers
#include <iostream>
#include <JANA/Services/JCalibrationManager.h>
#include <JANA/Calibrations/JCalibrationGeneratorCCDB.h>


int main(int narg, char *argv[])
{
	/*A.C. If I de-comment following, RootSPy won't work!*/
	//TH1::AddDirectory(kFALSE);

	// TODO: Bring back argparsing
	JApplication app;
	japp = &app;

	app.GetJParameterManager()->ReadConfigFile("config/bdx.config.BDXmini");
	app.SetParameterValue<std::string>("SYSTEM:OUTPUT", "root,out.root");
	app.SetParameterValue<std::string>("SYSTEM:VERBOSE", "1");
	app.SetParameterValue<std::string>("TT:XML_FILENAME", "/home/nbrei/src/bdxReco/tt.BDXmini.xml");
	app.SetParameterValue("nthreads", 2);
	app.SetParameterValue("jana:extended_report", 0);
	app.SetParameterValue("system:build_dst","CataniaProto2");
	app.Add("/home/nbrei/BDX_001098.evio.000");

	auto calib_manager = std::make_shared<JCalibrationManager>();
	calib_manager->AddCalibrationGenerator(new JCalibrationGeneratorCCDB); // TODO: Not quite happy with this
	app.ProvideService(calib_manager);
	app.ProvideService(std::make_shared<BDXCalibrationService>());

	//if(narg==1){
	//	app.Usage();
	//	return 0;
	//}
	app.ProvideService(std::make_shared<JGlobalRootLock>());

	app.Add(new JEventSourceEvioGenerator(&app));
	jout << "JEventSourceEvioGenerator DONE" << jendl;
	app.Add(new JEventSourceTRIDASGenerator(&app));
	jout<<"JEventSourceTRIDASGenerator DONE"<<jendl;
	app.Add(new JFactoryGenerator_system());
	jout<<"JFactoryGenerator_system DONE"<<jendl;
	app.Add(new JFactoryGenerator_DAQ());
	jout<<"JFactoryGenerator_DAQ DONE"<<jendl;
	app.Add(new JFactoryGenerator_EPICS());
	jout<<"JFactoryGenerator_EPICS DONE"<<jendl;
#ifdef MC_SUPPORT_ENABLE
	app.AddFactoryGenerator(new JFactoryGenerator_MC());
	jout<<"JFactoryGenerator_MC DONE"<<endl;
#endif
	app.Add(new JFactoryGenerator_TT());
	jout<<"JFactoryGenerator_TT DONE"<<jendl;
	app.Add(new JFactoryGenerator_ExtVeto());
	jout<<"JFactoryGenerator_ExtVeto DONE"<<jendl;
	app.Add(new JFactoryGenerator_IntVeto());
	jout<<"JFactoryGenerator_IntVeto DONE"<<jendl;
	app.Add(new JFactoryGenerator_Calorimeter());
	jout<<"JFactoryGenerator_Calorimeter DONE"<<jendl;
	app.Add(new JFactoryGenerator_Paddles());
	jout<<"JFactoryGenerator_Paddles DONE"<<jendl;
	app.Add(new JFactoryGenerator_BDXmini());
	jout<<"JFactoryGenerator_BDXmini DONE"<<jendl;
	app.Add(new JFactoryGenerator_EventBuilder());
	jout<<"JFactoryGenerator_EventBuilder DONE"<<jendl;
	app.Add(new BDXEventProcessor());
	jout<<"BDXEventProcessor DONE"<<jendl;
	app.Run();

	return 1;
}


