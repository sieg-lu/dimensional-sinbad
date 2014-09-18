// DimensionalSinbad.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "MainClass.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if(!MyLog::init()) {
		return -1;
	}

	MainClass app;
	app.go();

	return 0;
}

