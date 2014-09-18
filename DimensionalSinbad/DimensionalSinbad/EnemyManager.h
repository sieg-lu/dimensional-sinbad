#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include <vector>
#include "Blocks.h"
#include "Enemy.h"

struct EnemyData {
	int								x, y, z;
	std::vector<Job>				vCmd;
};

class EnemyManager {
private:
	std::vector<EnemyData>			vData;
	std::vector<Player*>			vEnemy;

public:
	//////////////////////////////////////////////////////////////////////////////
	// Basic functions
	//	-- Constructor / Destructor
	//	-- Inherited function
	//////////////////////////////////////////////////////////////////////////////
	EnemyManager(char* filename, Ogre::SceneManager* _inMgr, Blocks* block_ent);
	~EnemyManager();

	//////////////////////////////////////////////////////////////////////////////
	// Important functions
	//	-- Initialize
	//	-- Update
	//	-- Destroy
	//////////////////////////////////////////////////////////////////////////////
	virtual bool				Initialize();
	virtual bool				Update(const FrameEvent& evt, Player* pl);
	virtual bool				Destroy();

	//////////////////////////////////////////////////////////////////////////////
	// Custom functions
	//////////////////////////////////////////////////////////////////////////////
private:
	void						ReadXmlEnemyData(const char* filename);


public:
	std::vector<Player*>		getEnemy() const							{ return vEnemy; }

};


#endif ///> end of __ENEMY_MANAGER_H__