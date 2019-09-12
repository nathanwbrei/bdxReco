// $Id$
//
//    File: TranslationTable.h
// Created: Tue Jan 12 09:29:14 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _TranslationTable_
#define _TranslationTable_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <JANA/JException.h>
#include <JANA/JApplication.h>

#include <TObject.h>
#include <TString.h>

#include <set>
#include <map>


class TranslationTable: public JObject{

public:
	JOBJECT_PUBLIC(TranslationTable);

	TranslationTable(JApplication* app);
	~TranslationTable();

	// Each detector system has its own native indexing scheme.
	// Here, we define a class for each of them that has those
	// indexes. These are then used below in the DChannelInfo
	// class to relate them to the DAQ indexing scheme of
	// crate, slot, channel.
	struct csc_t : public TObject{
		uint8_t rocid;
		uint8_t slot;
		uint8_t channel;

		inline bool operator==(const struct csc_t &rhs) const {
			return (rocid == rhs.rocid) && (slot == rhs.slot)
					&& (channel == rhs.channel);
		}

		csc_t(){};
		virtual ~csc_t(){};
		void setCrateSlotChannel(uint8_t cr,uint8_t sl,uint8_t ch){rocid=cr;slot=sl;channel=ch;}
		//...................................
		// Less than operator for csc_t data types. This is used by
		// the map<csc_t, XX> to order the entires by key
		bool operator<(const TranslationTable::csc_t &b) const{
			if (rocid < b.rocid) return true;
			if (rocid > b.rocid) return false;
			if (slot < b.slot) return true;
			if (slot > b.slot) return false;
			if (channel < b.channel) return true;
			if (channel > b.channel) return false;
			return false;
		}
		string print()const{return Form("DAQ crate: %i slot: %i channel: %i",rocid,slot,channel);}

		ClassDef(csc_t,1);

	};

	enum Detector_t {
		UNKNOWN_DETECTOR,
		EXT_VETO,
		INT_VETO,
		CALORIMETER,
		PADDLES,
		NUM_DETECTOR_TYPES
	};

	string DetectorName(Detector_t type) const {
		switch (type) {
		case EXT_VETO:
			return "ExtVeto";
		case INT_VETO:
			return "IntVeto";
		case CALORIMETER:
			return "Calorimeter";
		case PADDLES:
			return "PADDLES";
		case UNKNOWN_DETECTOR:
		default:
			return "UNKNOWN";
		}
	}

	/*Here follows the classes to handle the IDs and intrinsic indexes of the different detectors
	 *
	 * There are some conventions on few indexes:
	 *
	 * - sector is an index regarding different, identical, detectors (in BDX we think about independent, but equal, detectors)
	 * - readout is an index to handle the case of different sensors reading out the same active volume
	 *
	 * Since we use these indexes both for the readout sensor and for the active volume, the convention is to use
	 * readout = 0 when dealing with the PHYSICAL volume
	 * readout = 1 ... Nreadouts when dealing with the sensors
	 * */
	class EXT_VETO_Index_t :  public TObject{
	public:
		int8_t sector;
		int8_t layer;
		int8_t component;
		int8_t readout;
		int8_t& ID(int n) {
			switch (n) {
			case 0:
				return sector;
				break;
			case 1:
				return layer;
				break;
			case 2:
				return component;
				break;
			case 3:
				return readout;
				break;
			default:
				jerr << "Wrong id" << std::endl;
				break;
			}
		}
		static const int nIDs() {
			return 4;
		};
		inline bool isSameActive(const EXT_VETO_Index_t &rhs) const {
			return (sector == rhs.sector) && (layer == rhs.layer)
					&& (component == rhs.component);
		}
		inline bool operator==(const EXT_VETO_Index_t &rhs) const {
			return isSameActive(rhs) && (readout == rhs.readout);
		}
		inline bool operator<(const EXT_VETO_Index_t &rhs) const { //A.C. for the maps
			if (sector > rhs.sector)
				return true;
			if (sector < rhs.sector)
				return false;
			if (layer > rhs.layer)
				return true;
			if (layer < rhs.layer)
				return false;
			if (component > rhs.component)
				return true;
			if (component < rhs.component)
				return false;
			if (readout > rhs.readout)
				return true;
			if (readout < rhs.readout)
				return false;
			return false;
		}
		string name()const{return "EXT_Veto";}
		string print()const{return Form("ExtVeto sector: %i layer: %i component: %i readout:%i",sector,layer,component,readout);}
		ClassDef(EXT_VETO_Index_t,1);

	};

	class INT_VETO_Index_t : public TObject{
	public:
		int8_t sector;
		int8_t layer;
		int8_t component;
		int8_t readout;
		int8_t& ID(int n) {
			switch (n) {
			case 0:
				return sector;
				break;
			case 1:
				return layer;
				break;
			case 2:
				return component;
				break;
			case 3:
				return readout;
				break;
			default:
				jerr << "Wrong id" << std::endl;
				break;
			}
		}
		static const int nIDs() {
			return 4;
		};
		inline bool isSameActive(const INT_VETO_Index_t &rhs) const {
			return (sector == rhs.sector) && (layer == rhs.layer)
					&& (component == rhs.component);
		}
		inline bool operator==(const INT_VETO_Index_t &rhs) const {
			return isSameActive(rhs) && (readout == rhs.readout);
		}
		inline bool operator<(const INT_VETO_Index_t &rhs) const { //A.C. for the maps
			if (sector > rhs.sector)
				return true;
			if (sector < rhs.sector)
				return false;
			if (layer > rhs.layer)
				return true;
			if (layer < rhs.layer)
				return false;
			if (component > rhs.component)
				return true;
			if (component < rhs.component)
				return false;
			if (readout > rhs.readout)
				return true;
			if (readout < rhs.readout)
				return false;
			return false;
		}
		string name()const{return "INT_Veto";}
		string print()const{return Form("IntVeto sector: %i layer: %i component: %i readout:%i",sector,layer,component,readout);}

		ClassDef(INT_VETO_Index_t,1);

	};

	class CALO_Index_t : public TObject{
	public:
		int8_t sector;
		int8_t x, y;
		int8_t readout;
		int8_t& ID(int n) {
			switch (n) {
			case 0:
				return sector;
				break;
			case 1:
				return x;
				break;
			case 2:
				return y;
				break;
			case 3:
				return readout;
				break;
			default:
				jerr << "Wrong id" << std::endl;
				break;
			}
		}
		static const int nIDs() {
			return 4;
		};
		inline bool isSameActive(const CALO_Index_t &rhs) const {
			return (sector == rhs.sector) && (x == rhs.x) && (y == rhs.y);
		}
		inline bool operator==(const CALO_Index_t &rhs) const {
			return isSameActive(rhs) && (readout == rhs.readout);
		}
		inline bool operator<(const CALO_Index_t &rhs) const { //A.C. for the maps
			if (sector > rhs.sector)
				return true;
			if (sector < rhs.sector)
				return false;
			if (x > rhs.x)
				return true;
			if (x < rhs.x)
				return false;
			if (y > rhs.y)
				return true;
			if (y < rhs.y)
				return false;
			if (readout > rhs.readout)
				return true;
			if (readout < rhs.readout)
				return false;
			return false;
		}
		string name()const{return "CALO";}
		string print()const{return Form("Calo sector: %i x: %i y: %i readout:%i",sector,x,y,readout);}
		ClassDef(CALO_Index_t,1);
	};

	class PADDLES_Index_t : public TObject{
	public:
		int8_t id;
		int8_t& ID(int n) {
			switch (n) {
			case 0:
				return id;
				break;
			default:
				jerr << "Wrong id" << std::endl;
				break;
			}
		}
		static const int nIDs() {
			return 1;
		};
		inline bool isSameActive(const PADDLES_Index_t &rhs) const {
			return (id == rhs.id);
		}
		inline bool operator==(const PADDLES_Index_t &rhs) const {
			return (id == rhs.id);
		}
		inline bool operator<(const PADDLES_Index_t &rhs) const { //A.C. for the maps
			if (id > rhs.id)
				return true;
			if (id < rhs.id)
				return false;

			return false;
		}
		string name() const{return "PADDLES";}
		string print()const{return Form("PADDLE id: %i",id);}
		ClassDef(PADDLES_Index_t,1);
	};

	/*A single class that handles ALL the possible indexes trough a C++ union*/
	class ChannelInfo : public TObject{
	public:
		csc_t CSC;   //This is crate - slot - channel
		//DModuleType::type_id_t module_type;
		Detector_t det_sys;
		union {
			EXT_VETO_Index_t *ext_veto;
			INT_VETO_Index_t *int_veto;
			CALO_Index_t *calorimeter;
			PADDLES_Index_t *paddles;
		};
		ClassDef(ChannelInfo,1);
	};

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {}

	void ReadTranslationTable(JCalibration *jcalib = NULL);

	/*Here goes the methods to return the channel info (detector name / detector-specific id) given the csc*/
	TranslationTable::ChannelInfo getChannelInfo(const csc_t &csc) const;
	TranslationTable::ChannelInfo getChannelInfo(int crate, int slot,
			int channel) const;
protected:
	string XML_FILENAME;
	bool NO_CCDB;
	std::set<string> supplied_data_types;
	int VERBOSE;
	string SYSTEMS_TO_PARSE;
	string ROCID_MAP_FILENAME;

	mutable JLogger ttout;

private:

	/****************************************** STATIC-VARIABLE-ACCESSING PRIVATE MEMBER FUNCTIONS ******************************************/

	//Some variables needs to be shared amongst threads (e.g. the memory used for the branch variables)
	//However, you cannot make them global/extern/static/static-member variables in the header file:
	//The header file is included in the TTAB library AND in each plugin that uses it
	//When a header file is included in a src file, it's contents are essentially copied directly into it
	//Thus there are two instances of each static variable: one in each translation unit (library)
	//Supposedly(?) they are linked together during runtime when loading, so there is (supposedly) no undefined behavior.
	//However, this causes a double free (double-deletion) when these libraries are closed at the end of the program, crashing it.
	//Thus the variables must be in a single source file that is compiled into a single library
	//However, you (somehow?) cannot make them global/extern variables in the cpp function
	//This also (somehow?) causes the double-free problem above for (at least) stl containers
	//It works for pointers-to-stl-containers and fundamental types, but I dunno why.
	//It's not good encapsulation anyway though.
	//THE SOLUTION:
	//Define the variables as static, in the source file, WITHIN A PRIVATE MEMBER FUNCTION.
	//Thus the static variables themselves only have function scope.
	//Access is only available via the private member functions, thus access is fully controlled.
	//They are shared amongst threads, so locks are necessary, but since they are private this class can handle it internally
	pthread_mutex_t& Get_TT_Mutex(void) const;
	bool& Get_TT_Initialized(void) const;
	std::map<TranslationTable::csc_t, TranslationTable::ChannelInfo>& Get_TT(
			void) const;

	ClassDef(TranslationTable,1);
};

#endif // _TranslationTable_

