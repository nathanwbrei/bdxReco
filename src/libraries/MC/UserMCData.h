#ifndef _UserMCData_
#define _UserMCData_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <MC/VetoMCHit.h>
#include "TObject.h"

/*This class is basically a place-holder for the data coming from EVIO file,
 * for the raw and digitized banks
 */
class UserMCData: public JObject, public TObject {
public:
	JOBJECT_PUBLIC(UserMCData);

	double data;
	int N;

	void Summarize(JObjectSummary& summary) const final {
		summary.add(N, "N", "%d");
		summary.add(data, "data", "%.6e");
	}
};

#endif

