#pragma once
#include "stdafx.h"
#include "Constants.h"


class Coin
{
	
public:
	//////////////////////////////////////////////////////////////////////////////
	// Basic functions
	//	-- Constructor / Destructor
	//	-- Inherited function
	//////////////////////////////////////////////////////////////////////////////
	Coin(String _name, Ogre::SceneManager* _mSceneMgr,  Ogre::Vector3 _initPos, Vector3 _absPos);
	~Coin();

	//////////////////////////////////////////////////////////////////////////////
	// Important functions
	//	-- Initialize
	//	-- Update
	//	-- Destroy
	//////////////////////////////////////////////////////////////////////////////
	void				Initialize();
	virtual bool		Update(const FrameEvent& evt);
	void				Destroy();

	////////////////// Variables
	String				name;
	Ogre::SceneNode*	node;
	Ogre::SceneManager* mSceneMgr;
	Vector3				initPos;
	Vector3				absPos;

	



};

