// C++ headers
#include <string>
using namespace std;

#include <JANA/JApplication.h>

// bdx headers
#include <system/JEventSourceEVIOGenerator.h>

#include <DAQ/JEventSourceEVIODAQ.h>

#ifdef MC_SUPPORT_ENABLE
#include <MC/JEventSourceEVIOMC.h>
#endif
JEventSourceEvioGenerator::JEventSourceEvioGenerator(JApplication* app) :
		isMC(0) {
#ifdef MC_SUPPORT_ENABLE
	gPARMS->SetDefaultParameter("MC", isMC, "Flag to select if analysis is runned on a MC file. 0 Means real data (default). >=1 means MC, according to: \n 1 CataniaProtoV1,\n 2 -> CataniaProtoV2,\n 4-> JLabFlux, \n 10->FullMC");
#endif
}


// Check if it's a valid file
double JEventSourceEvioGenerator::CheckOpenable(string source) {
	/*Return 1.0 if it contains "evio" string or the "ET:" string
	 */
	if (source.find("evio") != std::string::npos) {
		std::cout << "JEventSourceEvioGenerator: source name " << source << "contains \"evio\" substring. Open it" << endl;
		return 1.0;
	} else if (source.find("EVIO") != std::string::npos) {
		std::cout << "JEventSourceEvioGenerator: source name " << source << "contains \"EVIO\" substring. Open it" << endl;
		return 1.0;
	} else if (source.find("ET:") != std::string::npos) {
		std::cout << "JEventSourceEvioGenerator: source name " << source << "contains \"ET:\" substring . Open it" << endl;
		return 1.0;
	} else {
		std::cout << "JEventSourceEvioGenerator failed on source " << source << endl;
		return 0.0;
	}
}

// Make the file a JEventSource
JEventSource* JEventSourceEvioGenerator::MakeJEventSource(string source) {
	/*Here is the trick for now. Since we do not have the same file format for MC and for Data,
	 * but both are evio, let's look at the option MC, and see if it is 0 or 1
	 */
	int m_isMC, m_verbose;
	m_isMC = mApplication->GetParameterValue<int>("MC");
	m_verbose = mApplication->GetParameterValue<int>("SYSTEM:VERBOSE");

	if (m_isMC == 0) {
		if (m_verbose > 2) jout << "JEventSourceEvioGenerator::MakeJEventSource for DAQ " << endl;
		return new JEventSourceEvioDAQ(source.c_str(), mApplication);
	} else if (m_isMC >= 1) {

#ifdef  MC_SUPPORT_ENABLE
		if (m_verbose>2) jout<<" JEventSourceEvioGenerator::MakeJEventSource for MC "<<endl;
		return new JEventSourceEvioMC(source.c_str(), mApplication);
#else
		cerr << "=== ERROR: MC source specified and this was compiled without    ===" << endl;
		cerr << "===        MC support. You need to recompile setting MC=1 ,i.e. ===" << endl;
		cerr << "===        scons MC=1 								             ===" << endl;
		throw JException("Failed to open MC evio file - no MC support enabled");
#endif

	}

}

