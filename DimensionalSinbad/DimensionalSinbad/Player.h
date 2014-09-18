///////////////////////////////////////////////////////////////////////////////
// Player.h
//  -- Created: 11/18/2012
//	-- Author: Sieg.Lu (siegdt.lu@gmail.com)
//	-- Brief Description: 
//		This class inherits the ExampleFrameListener and SdkTrayListener,
//		and basically manage the keys and mouse events, and upper game logic
//  -- Update:
//      Sieg.Lu (11/19/2012)
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.h"
#include "ScoreManager.h"
#include "Blocks.h"
#include "Constants.h"
#include "Utility.h"
#include "CoinManager.h"

#define SCALE_RATIO 4.5f
#define ANGLE_STEP 5.0f

enum Job {
	eIdle = 0,
	eMovingForward,
	eTurningRight,
	eTurningLeft,
	eAttacking,
	eTeleporting,
	eWinning,
	eLosing,
};

class Player : public Object {

//protected:
public:
	Ogre::SceneManager*				_sceneMgr;
	Ogre::SceneNode*				_playerNode;
	Ogre::Entity*					_playerEnt;

	std::queue<Job>					_jobQueue;
//	Job								_currentJob;
	Ogre::AnimationState*			_aniStateTop;
	Ogre::AnimationState*			_aniStateBottom;

	float							_walkingSpeed;
	Ogre::Vector3					_destination;
	Ogre::Vector3					_step;
	Direction						_direction;
	// Facing North -- _angle = 0, in degree
	float							_currentAngle;
	float							_destAngle;

	bool							_isAttacking;
	Vector3							abs_position;
	Ogre::Vector3					initialPos;
	bool							animating;

	float							_moveAnimationSpeed;
	bool							_dead;
	bool							_win;
	CustomTimer						_winningTimer;
	int								_tgtTime;
	
	bool							shouldOverridePosition;
	Vector3							overridePos;
	Blocks*							blocks;
	Direction						camPos;

	CoinManager*					coinMgr;

	ScoreManager*					_scoreMgr;

protected:
	Player() { }

public:
	//////////////////////////////////////////////////////////////////////////////
	// Basic functions
	//	-- Constructor / Destructor
	//	-- Inherited function
	//////////////////////////////////////////////////////////////////////////////
	Player(Ogre::SceneManager* inMgr, ScoreManager* sMgr, Ogre::Vector3 initPos, Vector3 absPos);
	~Player();
	virtual std::string			ClassName()				{ return std::string("Player"); }
	virtual std::string			ToString()				{ return std::string(""); }

	//////////////////////////////////////////////////////////////////////////////
	// Important functions
	//	-- Initialize
	//	-- Update
	//	-- Destroy
	//////////////////////////////////////////////////////////////////////////////
	virtual bool				Initialize();
	// Update function return false means _jobQueue has just erase one front element
	// Used in Enemy class to indicate the next step
	virtual bool				Update(const FrameEvent& evt, Player* enemies);
	virtual bool				Update(const FrameEvent& evt, std::vector<Player*> enemies);
	virtual bool				Destroy();

	//////////////////////////////////////////////////////////////////////////////
	// Custom functions
	//////////////////////////////////////////////////////////////////////////////
public:
	void						MoveForward();
	virtual void				TurnRight();
	virtual void				TurnLeft();
	virtual void				DoAttack();
	void						Move(Direction dir, Blocks *blocks, Direction _camPos, Vector3 newPos = NULL_VECTOR);
	void						DimensionalUpdate(Direction _dir, Blocks *_blocks);
	Vector3						getPlayerPosition();
	void						Winning(int danceTimes);
	bool						IsDead() const				{ return _dead; }

protected:
	bool						Teleport(const FrameEvent& evt);
	void						UpdateAbsPosition();
	virtual bool				MoveForwardImpl(const FrameEvent& evt);
	bool						TurnRightImpl(const FrameEvent& evt);
	bool						TurnLeftImpl(const FrameEvent& evt);
	virtual bool				DoAttackImpl(const FrameEvent& evt);
	virtual bool				AttackEnemy(Player* other);
	virtual void				Killed();
	bool						WinningImpl(const FrameEvent& evt);

};

