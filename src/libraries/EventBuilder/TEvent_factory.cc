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
void TEvent_factory::Init()
{
}

//------------------
// brun
//------------------
void TEvent_factory::ChangeRun(const std::shared_ptr<const JEvent>& event)
{
}

//------------------
// evnt
//------------------
void TEvent_factory::Process(const std::shared_ptr<const JEvent>& event)
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
}

//------------------
// erun
//------------------
void TEvent_factory::EndRun()
{
}

//------------------
// fini
//------------------
void TEvent_factory::Finish()
{
}

