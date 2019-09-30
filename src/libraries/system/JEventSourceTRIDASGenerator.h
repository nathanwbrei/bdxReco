#ifndef _JEventSourceGenerator_TRIDAS_
#define _JEventSourceGenerator_TRIDAS_

// JANA headers
#include "JANA/JEventSourceGenerator.h"

class JEventSourceTRIDASGenerator: public JEventSourceGenerator{

	public:

		explicit JEventSourceTRIDASGenerator(JApplication* app) {}

		~JEventSourceTRIDASGenerator() override = default;

		std::string GetType() const override { return "JEventSourceTRIDASGenerator"; }
		
		std::string GetDescription() const override;

		double CheckOpenable(std::string source) override;

		JEventSource* MakeJEventSource(std::string source) override;
};

#endif // _JEventSourceGenerator_TRIDAS_

