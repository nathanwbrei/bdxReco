// $Id$
//
//    File: TEvent_factory.cc
// Created: Thu Dec 22 12:32:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "TEvent_factory.h"

//------------------
// init
//------------------
jerror_t TEvent_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t TEvent_factory::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t TEvent_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// TEvent *myTEvent = new TEvent;
	// myTEvent->x = x;
	// myTEvent->y = y;
	// ...
	// _data.push_back(myTEvent);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t TEvent_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t TEvent_factory::fini(void)
{
	return NOERROR;
}

