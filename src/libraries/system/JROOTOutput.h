// $Id$
//
//    File: JOutput.h
// Created: Tue Jan 12 14:45:40 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JRootOutput_
#define _JRootOutput_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "JOutput.h"

#include <string>

//ROOT headers
#include "TFile.h"
#include "TObject.h"
#include "TList.h"
#include "TCollection.h"

using namespace std;

class JROOTOutput : public JOutput{
	public:
		JOBJECT_PUBLIC(JROOTOutput);
		
		JROOTOutput();
		~JROOTOutput();

		int AddObject(TObject *obj);
		int SaveAll();
		virtual int CloseOutput();
		virtual int OpenOutput(std::string name);

	private:
		TFile *m_file;
		TList *m_objects;
		TObject *m_cur_obj;
};

#endif // _JOutput_

