#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Player.h"

#define ENEMY_SCALE_RATIO 0.2f

class Enemy : public Player {
private:
	std::vector<Job>			_cmdVector;
	int							_cmdIndex;

public:
	//////////////////////////////////////////////////////////////////////////////
	// Basic functions
	//	-- Constructor / Destructor
	//	-- Inherited function
	//////////////////////////////////////////////////////////////////////////////
	Enemy(Ogre::SceneManager* inMgr,  Ogre::Vector3 initPos, Vector3 absPos);
	~Enemy();
	virtual std::string			ClassName()				{ return std::string("Enemy"); }
	virtual std::string			ToString()				{ return std::string(""); }

	//////////////////////////////////////////////////////////////////////////////
	// Important functions
	//	-- Initialize
	//	-- Update
	//	-- Destroy
	//////////////////////////////////////////////////////////////////////////////
	virtual bool				Initialize();
	virtual bool				Update(const FrameEvent& evt, Player* enemy);
	virtual bool				Destroy();

	//////////////////////////////////////////////////////////////////////////////
	// Custom functions
	//////////////////////////////////////////////////////////////////////////////
public:
	virtual void				DoAttack();
	virtual void				TurnRight();
	virtual void				TurnLeft();

	void						InitCmdVector();
	void						InitCmdVector(std::vector<Job> vv);

	void						NextCmd();

protected:
	virtual bool				DoAttackImpl(const FrameEvent& evt);
	virtual bool				AttackEnemy(Player* other);
	virtual void				Killed();
	virtual bool				KilledImpl(const FrameEvent& evt);
	virtual bool				MoveForwardImpl(const FrameEvent& evt);
	bool run;
};


#endif /// end of __ENEMY_H__