#ifndef _JEventSourceGenerator_TRIDAS_
#define _JEventSourceGenerator_TRIDAS_

// JANA headers
#include "JANA/JEventSourceGenerator.h"

class JEventSourceTRIDASGenerator: public JEventSourceGenerator{
	public:
		JEventSourceTRIDASGenerator(){}
		virtual ~JEventSourceTRIDASGenerator(){}


		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JEventSourceTRIDASGenerator";}
		
		const char* Description(void);
		double CheckOpenable(string source);

		JEventSource* MakeJEventSource(string source);
};

#endif // _JEventSourceGenerator_TRIDAS_

