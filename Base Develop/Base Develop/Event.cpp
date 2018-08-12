#include "stdafx.h"
#include "Event.h"


Event::Event()
{
}

Event::~Event()
{
	CloseHandle(_event);
}

void Event::WaitSignal()
{
	WaitForSingleObject(_event, INFINITE);
}