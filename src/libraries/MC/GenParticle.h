// $Id$
//
//    File: genParticle.h
// Created: Tue Mar  1 00:59:14 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _GenParticle_
#define _GenParticle_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include "TObject.h"

class GenParticle : public JObject, public TObject {
	public:

		GenParticle(){
		}
		JOBJECT_PUBLIC(GenParticle);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			 AddString(items, "id", "%4d", id);
			 AddString(items, "pid", "%4d", pid);
			 AddString(items, "px", "%4f", px);
			 AddString(items, "py", "%4f", py);
			 AddString(items, "pz", "%4f", pz);
			 AddString(items, "vx", "%4f", vx);
			 AddString(items, "vy", "%4f", vy);
			 AddString(items, "vz", "%4f", vz);
		}



		double px;
		double py;
		double pz;
        int pid;
        double vx;
        double vy;
        double vz;


        ClassDef(GenParticle,1);

};

#endif // _genParticle_

