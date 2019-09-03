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
	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string, string> > &items) const {
		AddString(items, "N", "%d", N);
		AddString(items, "data", "%.6e", data);
	}
};

#endif

