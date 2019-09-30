// C++ headers
#include <string>
using namespace std;

// bdx headers
#include <system/JEventSourceTRIDASGenerator.h>
#include <DAQ/JEventSourceTRIDASDAQ.h>
#include <JANA/JApplication.h>


//---------------------------------
// Description
//---------------------------------
std::string JEventSourceTRIDASGenerator::GetDescription() const {
	return "TRIDAS";
}

//---------------------------------
// CheckOpenable
//---------------------------------
double JEventSourceTRIDASGenerator::CheckOpenable(string source) {
	// This should return a value between 0 and 1 inclusive
	// with 1 indicating it definitely can read events from
	// the specified source and 0 meaning it definitely can't.
	// Typically, this will just check the file suffix.

	if (source.find("pt") != std::string::npos) {
		std::cout << "JEventSourceTRIDASGenerator: source name " << source << " contains \"pt\" substring. Open it" << endl;
		return 1.0;
	} else {
		std::cout << "JEventSourceTRIDASGenerator failed on source " << source << endl;
		return 0.0;
	}
}

//---------------------------------
// MakeJEventSource
//---------------------------------
JEventSource* JEventSourceTRIDASGenerator::MakeJEventSource(string source) {

	int m_isMC, m_verbose;
	m_isMC = 0;
	m_verbose = 0;
	m_isMC = mApplication->GetParameterValue<int>("MC");
	m_verbose = mApplication->GetParameterValue<int>("SYSTEM:VERBOSE");

	if (m_isMC == 0) {
		if (m_verbose > 2) jout << "JEventSourceTRIDASGenerator::MakeJEventSource for DATA " << jendl;
		return new JEventSourceTRIDASDAQ(source.c_str(), mApplication);
	} else if (m_isMC >= 1) {
		jerr << "MC - TRIDAS format not yet supported!!" << jendl;
		throw JException("Failed to open MC TRIDAS file - no MC support yet");
	}
}

