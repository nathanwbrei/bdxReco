#ifndef _JEventSourceEvioGenerator_
#define _JEventSourceEvioGenerator_

// JANA headers
#include "JANA/JEventSourceGenerator.h"

class JEventSourceEvioGenerator : public JEventSourceGenerator {
private:
    int isMC;

public:
    explicit JEventSourceEvioGenerator(JApplication* app);

    ~JEventSourceEvioGenerator() override = default;

    std::string GetType() const override { return "JEventSourceEvioGenerator"; }

    std::string GetDescription() const override { return "Evio"; }

    double CheckOpenable(std::string source) override;

    JEventSource* MakeJEventSource(std::string source) override;

};

#endif // _JEventSourceEvioGenerator_

