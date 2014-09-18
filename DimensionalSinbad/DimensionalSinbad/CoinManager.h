#pragma once
#include "stdafx.h"
#include "ScoreManager.h"
#include "Constants.h"
#include "Coin.h"
#include "Blocks.h"
//#include "Player.h"

using namespace std;

class CoinManager
{
public:
	CoinManager(Ogre::SceneManager* _SceneMgr, ScoreManager* sMgr, string _fileName, Blocks* _blocks_ent);
	~CoinManager(void);


	//////////////////////////////////////////////////////////////////////////////
	// Important functions
	//	-- Initialize
	//	-- Update
	//	-- Destroy
	//////////////////////////////////////////////////////////////////////////////
	void				Initialize();
	virtual bool		Update(const FrameEvent& evt);
	void				Destroy();
	//////////////////////////////////////////////////////////////////////////////

	bool readLevelsFile(string filename);
	bool CheckForCoins(Vector3 abs_position, Direction camPos);
	void GenerateCoins();


	///////////////// Variables
	//Player*					mPlayer;
	Ogre::SceneManager*		mSceneMgr;
	string					fileName;
	Blocks*					blocks_ent;
	bool					isRunning;
	std::vector<Vector3>	coinVectors;
	std::vector<Coin*>		coinList;
	
	ScoreManager*			_scoreMgr;



};

