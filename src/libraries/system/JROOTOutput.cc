#include "JROOTOutput.h"

JROOTOutput::JROOTOutput() :
		m_file(0), m_objects(0), m_cur_obj(0) {
	jout << "JRootOutput creator called" << jendl;

}

JROOTOutput::~JROOTOutput() {
  if (m_file) {
  		delete m_file;
		m_file=0;
  	}
	if (m_objects) {
		delete m_objects;
		m_objects=0;
	 }
  
  jout<<"JRootOutput destructor ends"<<jendl;
  fflush(stdout);
}

int JROOTOutput::OpenOutput(string fname) {
	jout << "JROOTOutput::OpenOutput is called with filename: " << fname << jendl;
	if (m_isOpen == 0) {
		m_file = new TFile(fname.c_str(), "RECREATE");
		m_objects = new TList();
		m_isOpen = 1;
	} else {
		jout << "Already opened, nothing to do" << jendl;
	}
}

int JROOTOutput::CloseOutput() {
  jout << "JROOTOutput::CloseOutput is called m_isOpen:" <<m_isOpen<< jendl;
  fflush(stdout);
	if (m_isOpen!=0) {
	  	m_isOpen = 0;
		this->SaveAll();
		m_file->Write();
		m_file->Close();
	
	} else {
		jout << "Already closed, nothing to do" << jendl;
	}
	return 0;
}

int JROOTOutput::SaveAll() {
	if (!m_objects) {
		jerr << "no m_objects, doing nothing" << jendl;
		fflush(stdout);
		return -1;
	}
	if (m_file==0) {
		jerr << "no m_file, doing nothing" << jendl;
		fflush(stdout);
		return -2;
	}

	TIter m_iter(m_objects);
	m_file->cd();
	//Loop over the TList via a TIter
	while ((m_cur_obj = m_iter.Next())) {
		if (m_cur_obj != 0) {
			jout << "JROOTOutput::saving object " << m_cur_obj->GetName() << jendl;
			fflush(stdout);
			m_cur_obj->Write();
		}
	}
	return 0; // prevent compiler warnings
}

int JROOTOutput::AddObject(TObject *obj) {
	if (!m_objects) {
		jerr << "no m_objects, doing nothing" << jendl;
		return -1;
	}
	m_objects->Add(obj);
	return 0; // prevent compiler warnings
}
