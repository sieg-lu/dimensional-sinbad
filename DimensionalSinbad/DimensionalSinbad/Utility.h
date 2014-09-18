///////////////////////////////////////////////////////////////////////////////
// Utility.h
//  -- Last updated: 10/30/2012
//	-- Author: Sieg.Lu (siegdt.lu@gmail.com)
//	-- Brief Description: 
//		Basic utility
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"



static class MyLog
{
public:
	static bool init()
	{
		std::ofstream fout = std::ofstream("Dimensional_Sinbad_Log.txt");
		if (!fout) {
			return false;
		}
		fout.close();
		return true;
	}

	static void put(int line, char* file, char* msg)
	{
		std::ofstream fout("Dimensional_Sinbad_Log.txt", std::ios::app);
//		std::cout << "Line " << line << " in file: " << file << " --" << msg << std::endl;
		fout << "Line " << line << " in file: " << file << " --" << msg << std::endl;
		fout.close();
	}

};

class CustomTimer
{
public:
	CustomTimer() : _startTime(clock())			{ }
	void Start()								{ _startTime = clock(); }
	int CurrentTime()							{ return clock() - _startTime; }

private:
	clock_t _startTime;
};

bool FloatEqual(float a, float b, double eps = 1e-6);
float Degree2Radian(float deg);
float Radian2Degree(float rad);

