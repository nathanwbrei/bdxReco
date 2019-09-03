/*
 * JOutput.h
 *
 *  Created on: Jan 13, 2016
 *      Author: celentan
 */

#ifndef JOUTPUT_H_
#define JOUTPUT_H_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "TObject.h"

#include <string>

class JOutput: public JObject, public TObject {
protected:
	int m_isOpen;
public:
	JOBJECT_PUBLIC(JOutput)	;

	JOutput() : m_isOpen(0) {};

	//This virtual method has to be implemented in derived classes, specifying what to do when "fini" is called in the BDXEventProcess

	virtual int CloseOutput()=0;
	virtual int OpenOutput(std::string name)=0;

	inline int isOpen() {
		return m_isOpen;
	}

};

#endif /* JOUTPUT_H_ */
