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


		void Summarize(JObjectSummary& summary) {
			 summary.add(id, "id", "%4d");
			 summary.add(pid, "pid", "%4d");
			 summary.add(px, "px", "%4f");
			 summary.add(py, "py", "%4f");
			 summary.add(pz, "pz", "%4f");
			 summary.add(vx, "vx", "%4f");
			 summary.add(vy, "vy", "%4f");
			 summary.add(vz, "vz", "%4f");
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

