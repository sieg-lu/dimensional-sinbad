///////////////////////////////////////////////////////////////////////////////
// EntryClass.h
//  -- Last updated: 10/30/2012
//	-- Author: Sieg.Lu (siegdt.lu@gmail.com)
//	-- Brief Description: 
//		This class inherits the ExampleFrameListener and SdkTrayListener,
//		and basically manage the keys and mouse events, and upper game logic
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.h"
#include "Constants.h"
#include "EnemyManager.h"
#include "Blocks.h"
#include "Enemy.h"
#include "CoinManager.h"
#include "ScoreManager.h"

class MainClass : public BaseApplication
{

public:
	MainClass();
	~MainClass();

private:

	virtual void createCamera(void);
    virtual void createScene(void);
    void createScene2(std::string filename);
    void createScene3(void);
    virtual void createViewports(void);

	virtual void createFrameListener();

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    //virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	void movementKeyPressed(void);
	Vector3 GetOrientationVector(Direction dir, bool collisionCheck);
	
	void moveCamera( int _x, int _y );	
	Vector3 FindNextPosition(Direction dir, bool camIsTop = false);

	bool	initialized;
	Blocks* blocks_ent;
	Player*	_player;
// 	Enemy* _enemy;
	EnemyManager* _enemyMgr;

	int score;
	std::vector<String> levelsFile;

	OIS::KeyCode movementKey;
	
	Direction camera_ePosition;
	Direction eW_direction;
	Direction eA_direction;
	Direction eS_direction;	
	Direction eD_direction;	

	bool _winning;
	bool _gameover;
	OgreBites::Label* mInfoLabel;
	ScoreManager* _scoreMgr;
	int _totalScore;
	bool _timerAlreadySet;
};
