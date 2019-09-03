#ifndef _JEventSourceEvioGenerator_
#define _JEventSourceEvioGenerator_

// JANA headers
#include "JANA/JEventSourceGenerator.h"

class JEventSourceEvioGenerator:public JEventSourceGenerator
{
private:
	int isMC;
	public:
		 JEventSourceEvioGenerator();
		~JEventSourceEvioGenerator(){}


		const char* className(void)               {return static_className();}
		static const char* static_className(void) {return "JEventSourceEvioGenerator";}
		
		const char* Description(void);
		
		double CheckOpenable(string source);
		
		JEventSource* MakeJEventSource(string source);
		

};

#endif // _JEventSourceEvioGenerator_

