///////////////////////////////////////////////////////////////////////////////
// Object.h
//  -- Last updated: 10/31/2012
//	-- Author: Sieg.Lu (siegdt.lu@gmail.com)
//	-- Brief Description: 
//		Base object class for all the objects in the game
//		Every object in the game must inherit this class
//		and implement the virtual functions
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.h"

class Object {
public:
	//////////////////////////////////////////////////////////////////////////////
	// Important functions
	//	-- Initialize
	//	-- Update
	//	-- Destroy
	//////////////////////////////////////////////////////////////////////////////
	virtual bool				Initialize()						= 0;
	virtual bool				Update(const FrameEvent& evt)		{ return true; }
	virtual bool				Destroy()							= 0;

	//////////////////////////////////////////////////////////////////////////////
	// Identification functions
	//	-- ClassName
	//	-- ToString
	//////////////////////////////////////////////////////////////////////////////
	virtual std::string			ClassName()							= 0;
	virtual std::string			ToString()							= 0;

};
