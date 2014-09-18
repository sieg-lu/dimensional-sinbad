#include "stdafx.h"
#include "Utility.h"

bool FloatEqual(float a, float b, double eps /*= 1e-6*/)
{
	return (abs(a - b) < eps);
}

float Degree2Radian(float deg)
{
	return (deg * Ogre::Math::PI / 180.0f);
}

float Radian2Degree(float rad)
{
	return (rad * 180.0f / Ogre::Math::PI);
}
