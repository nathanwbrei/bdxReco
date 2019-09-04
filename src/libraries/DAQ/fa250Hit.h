/*
 * fa250Hit.h
 *
 *  Created on: Jan 25, 2016
 *      Author: celentan
 */

#ifndef FA250HIT_H_
#define FA250HIT_H_

//#include <TT/TranslationTable.h>
#include <system/BDXObject.h>
#include "TObject.h"

namespace TranslationTable {
struct csc_t : public TObject {
	uint8_t rocid;
	uint8_t slot;
	uint8_t channel;

	inline bool operator==(const struct csc_t& rhs) const {
		return (rocid == rhs.rocid) && (slot == rhs.slot)
			   && (channel == rhs.channel);
	}

	csc_t() {};

	virtual ~csc_t() {};

	void setCrateSlotChannel(uint8_t cr, uint8_t sl, uint8_t ch) {
		rocid = cr;
		slot = sl;
		channel = ch;
	}
	//...................................
	// Less than operator for csc_t data types. This is used by
	// the map<csc_t, XX> to order the entires by key
	bool operator<(const TranslationTable::csc_t& b) const {
		if (rocid < b.rocid) return true;
		if (rocid > b.rocid) return false;
		if (slot < b.slot) return true;
		if (slot > b.slot) return false;
		if (channel < b.channel) return true;
		if (channel > b.channel) return false;
		return false;
	}

	string print() const { return "DAQ crate: %i slot: %i channel: %i"; }

ClassDef(csc_t, 1);

};
} // namespace TT

class fa250Hit : public BDXObject{
public:


	TranslationTable::csc_t m_channel;
	int trigger;
	uint64_t timestamp;

	ClassDef(fa250Hit,1)
};

#endif /* FA250HIT_H_ */
