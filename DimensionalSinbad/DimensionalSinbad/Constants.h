/*
Containts all the constants 

*/#pragma once
#include "stdafx.h"

class Constants
{
public:
	Constants(void);
	~Constants(void);	
};


enum Direction {
	eNorth = 0,
	eWest,
	eSouth,
	eEast,
	eNONE,
	eTop
};

enum blockColor 
{
	BLOCK_COLOR_WHITE = 1, 
	BLOCK_COLOR_GREEN = 2, 
	BLOCK_COLOR_RED	  = 3
};

	// For block searching
	const static Vector3 NULL_VECTOR = Vector3(-999,-999,-999);
	
	// Coin 
	const static Ogre::String COIN_NAME = "Coin";
	const static Ogre::String COIN_MATERIAL_NAME = "Coin";
	const static Real COIN_SCALE = 0.1;

	// Viewport
	const static Ogre::ColourValue VIEWPORT_COLOR	= Ogre::ColourValue(0, 0, 0);

	// Camera
	const static int CAM_DISTANCE = 1000;
	const static Ogre::String CAMERA_NAME	= "Cam";
	const static Ogre::Vector3 CAMERA_POS	= Ogre::Vector3(0,0,0);
	const static Ogre::Vector3 CAMERA_LOOKAT= Ogre::Vector3(0,0,0);
	const static OgreBites::CameraStyle CAMERA_STYLE = OgreBites::CS_ORBIT;
	const static int CAMERA_CLIP_DIST = 1;

	// Point light
	const static Ogre::Light::LightTypes POINT_LIGHT_TYPE = Ogre::Light::LT_SPOTLIGHT;
	const static Ogre::String POINT_LIGHT_NAME	= "pointLight";
	const static Ogre::Vector3 POINT_LIGHT_POS	= Ogre::Vector3(0, 100, 0);
	const static Ogre::ColourValue POINT_LIGHT_DIFF_COLOR = Ogre::ColourValue(0,0.5,0);
	const static Ogre::ColourValue POINT_LIGHT_SPEC_COLOR = Ogre::ColourValue(0,0.5,0);

	// Blocks
	const std::string GREEN_BLOCK_MATERIAL = "Template/GreenShaded";
	const std::string RED_BLOCK_MATERIAL = "Template/RedShaded";
	const std::string WHITE_BLOCK_MATERIAL = "Template/WhiteShaded";