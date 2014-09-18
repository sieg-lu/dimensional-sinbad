#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass():
	blocks_ent(0),
	_player(0),
	_enemyMgr(0)
{	
//	MyLog::put(__LINE__, __FILE__, "MainClass Initializing.\n");
	_scoreMgr = new ScoreManager(1000000, 1200000);
	movementKey = OIS::KC_0;
	camera_ePosition = eNONE;
	score = 0;
	_winning = false;
	_gameover = false;
	_totalScore = 0;
	_timerAlreadySet = false;
}

MainClass::~MainClass()
{
	if (initialized){
		delete _player;
		delete _enemyMgr;

		delete mCameraMan;
		if (_scoreMgr) {
			delete _scoreMgr;
		}
		mCameraMan = 0;
	}
}


// Gives access to the game loop
bool MainClass::frameRenderingQueued(const FrameEvent& evt)
{
	if(!BaseApplication::frameRenderingQueued(evt))
	{
		return false;
	}
//	std::cout << _scoreMgr->getCurrentTime() << std::endl;
	if (_gameover && _scoreMgr->getCurrentTime() >= 2000) {
		_gameover = false;
		_timerAlreadySet = false;
		createScene2(levelsFile.at(score));
	}
	// GameLogic->Update();
	if (_player->IsDead() 
		|| blocks_ent->isWin(camera_ePosition, _player->getPlayerPosition()) == 2) {
		// Game Over Event
		mInfoLabel->setCaption("Game Over ...");
		_gameover = true;
		if (!_timerAlreadySet) {
			_scoreMgr->GameStart();
			_timerAlreadySet = true;
		}
		return true;
	} else {
		char tmp[20];
		itoa((_totalScore + _scoreMgr->getScore()) * 10, tmp, 10);
		mInfoLabel->setCaption(tmp);
	}
	bool playerWinAniFinished = !_player->Update(evt, _enemyMgr->getEnemy());
	_enemyMgr->Update(evt, _player);
	movementKeyPressed();
	// test win lose
	int isWin = blocks_ent->isWin(camera_ePosition, _player->getPlayerPosition());

	if (isWin == 1 && !_winning) {
		_player->Winning(3000);
		_winning = true;
	}

	if (isWin == 1 && playerWinAniFinished) {
		score++;
		// show win or lose info
		printf("winning");
		_winning = false;
		_totalScore += _scoreMgr->getScore();
		if (score < levelsFile.size() && levelsFile.at(score) != "") {
			createScene2(levelsFile.at(score));
		}
	}

	mCameraMan->frameRenderingQueued(evt); 

}

void MainClass::createFrameListener()
{
	BaseApplication::createFrameListener();

	mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOPLEFT, "Score:", "", 220);
	_scoreMgr->GameStart();
}

void MainClass::createScene(void)
{
	Ogre::Light* directionalLight = mSceneMgr->createLight("directionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(Ogre::ColourValue(.25, .25, 0));
    directionalLight->setSpecularColour(Ogre::ColourValue(.25, .25, 0));
	directionalLight->setDirection(Ogre::Vector3( 0.5, -1, 1 ));

	// read the names of xml files that contain the blocks information
	levelsFile = Blocks::readLevelsFile("levels.xml");

	// use the first level file to crate blocks
	blocks_ent = new Blocks(levelsFile.front(), mSceneMgr);
	

	Vector3 startingPos = Blocks::getPlayerStartPos(levelsFile.front());// Vector3(4,1,0);
	Vector3 pos = blocks_ent->absToRealPos(startingPos);
	_player = new Player(mSceneMgr, _scoreMgr, pos, startingPos);
	_player->Initialize();

	_enemyMgr = new EnemyManager("enemy.xml", mSceneMgr, blocks_ent);
	_enemyMgr->Initialize();

	initialized = true;
	
	CoinManager* cm = new CoinManager(mSceneMgr, _scoreMgr, levelsFile.front(), blocks_ent);
	_player->coinMgr = cm;
	cm->Initialize();
}


void MainClass::createScene2(std::string filename)
{
	delete blocks_ent;
	delete _player;
	delete _enemyMgr;
	mSceneMgr->destroyAllEntities();

	
	mCameraMan->setYawPitchDist(Ogre::Radian(0.5), Ogre::Radian(0.5), CAM_DISTANCE);
	camera_ePosition = eNONE;

	blocks_ent = new Blocks(filename, mSceneMgr);

	_enemyMgr = new EnemyManager("enemy2.xml", mSceneMgr, blocks_ent);
	_enemyMgr->Initialize();

	Vector3 startingPos = Blocks::getPlayerStartPos(filename);// Vector3(4,1,0);
	Vector3 pos = blocks_ent->absToRealPos(startingPos);
//	_player = new Enemy(mSceneMgr, Ogre::Vector3(pos.x, pos.y-25, pos.z), startingPos);
	_player = new Player(mSceneMgr, _scoreMgr, pos, startingPos);
	_player->Initialize();

	CoinManager* cm = new CoinManager(mSceneMgr, _scoreMgr, filename, blocks_ent);
	_player->coinMgr = cm;
	cm->Initialize();
}

void MainClass::createCamera(void) 
{
	// Create the camera
    mCamera = mSceneMgr->createCamera(CAMERA_NAME);
    mCamera->setPosition(CAMERA_POS);
	mCamera->lookAt(CAMERA_LOOKAT);
	mCamera->setNearClipDistance(CAMERA_CLIP_DIST);
	mCamera->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
	mCameraMan->setStyle(CAMERA_STYLE);
	mCameraMan->setYawPitchDist(Ogre::Radian(0.5), Ogre::Radian(0.5), CAM_DISTANCE);
	eW_direction = eNorth;
	eS_direction = eSouth;
	eA_direction = eWest;
	eD_direction = eEast;
}


void MainClass::createViewports(void) 
{
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(VIEWPORT_COLOR);
		// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));  
}


// Snaps the camera to an angle, if the conditions are true
// Snapping the camera also adjusts the player's position
bool MainClass::mouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id )
{	

	if (id == OIS::MB_Left)
	{
		Ogre::Radian my = mCamera->getOrientation().getYaw();
		std::cout << "\n"<< my;
		// Check pitch angle first
		if (mCamera->getDirection().y > -0.08 && mCamera->getDirection().y < 0.08)
		{
			mCamera->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
			// Then Yaw angle
			if (my >  Ogre::Radian(1.3) && my < Ogre::Radian(1.9))
			{
				camera_ePosition = eWest;
				_player->DimensionalUpdate(camera_ePosition, blocks_ent);

				eW_direction = eNONE;
				eS_direction = eNONE;
				eA_direction = eSouth;
				eD_direction = eNorth;
				mCameraMan->setYawPitchDist(Ogre::Radian(Ogre::Math::PI/2), Ogre::Radian(0), CAM_DISTANCE);
			}
			else if (my >  Ogre::Radian(-0.3) && my < Ogre::Radian(0.3))
			{
				camera_ePosition = eSouth;
				_player->DimensionalUpdate(camera_ePosition, blocks_ent);
				eW_direction = eNONE;
				eS_direction = eNONE;
				eA_direction = eWest;
				eD_direction = eEast;
				mCameraMan->setYawPitchDist(Ogre::Radian(0),  Ogre::Radian(0), CAM_DISTANCE);
			}
			else if (my <  Ogre::Radian(-1.3) && my > Ogre::Radian(-1.9))
			{
				camera_ePosition = eEast;
				_player->DimensionalUpdate(camera_ePosition, blocks_ent);
				eW_direction = eNONE;
				eS_direction = eNONE;
				eA_direction = eNorth;
				eD_direction = eSouth;
				mCameraMan->setYawPitchDist(Ogre::Radian(-Ogre::Math::PI/2),  Ogre::Radian(0), CAM_DISTANCE);
			}
			else if (my >  Ogre::Radian(2.9) || my < Ogre::Radian(-2.9))
			{
				camera_ePosition = eNorth;
				_player->DimensionalUpdate(camera_ePosition, blocks_ent);
				eW_direction = eNONE;
				eS_direction = eNONE;
				eA_direction = eEast;
				eD_direction = eWest;
				mCameraMan->setYawPitchDist(Ogre::Radian(-Ogre::Math::PI), Ogre::Radian(0), CAM_DISTANCE);
			}
		}

	}
	mCameraMan->injectMouseUp(arg,id);
	return true;
}


void MainClass::moveCamera( int _x, int _y )
{
	Ogre::Real dist = (mCamera->getPosition() - mCameraMan->getTarget()->_getDerivedPosition()).length();

		mCamera->setPosition(mCameraMan->getTarget()->_getDerivedPosition());
		
		mCamera->yaw(Ogre::Degree(-_x * 0.25f));
		Ogre::Radian y = mCamera->getOrientation().getYaw();

		bool side = false;
		// Side is true, if the camera is adjusting Pitch Angle on the Z Axis
		if ( (y <= Ogre::Radian(Math::PI * 3)/4 && y >= Ogre::Radian(Math::PI * 1)/4 ) ||
			 (y >= Ogre::Radian(Math::PI * 3)/-4 && y <= Ogre::Radian(Math::PI * 1)/-4 ) )
			 side = true;

			// The following conditions limit the Pitch of the Camera
			// as well as compensating for a weird angle issue when adjusting the Pitch from the Z Axis
			////////////////////////////////////
			Ogre::Radian p = mCamera->getOrientation().getPitch();
			Ogre::Degree p2 = Ogre::Degree(-_y * 0.15f);
			Real p2f = -_y * 0.15f;
			int limit = 3;
			if (side && p2f < -limit) p2f = -limit;
			else if (side && p2f > limit) p2f =  limit;

			p2 = Ogre::Degree(p2f);
			if (p > Ogre::Degree(0)) p2 = -p2;
			Ogre::Degree np = p + p2;

			if ( (np < Ogre::Degree(90) && np > Ogre::Degree(-90) && !side) || 
				 (side && mCamera->getDirection().y > -0.999) ||
				 (side && mCamera->getDirection().y < -0.99 && p > Ogre::Degree(0) &&  p2 < Ogre::Degree(0)) ||
				 (side && mCamera->getDirection().y < -0.99 && p < Ogre::Degree(0) &&  p2 > Ogre::Degree(0)) )
			{
				mCamera->pitch(Ogre::Degree(p2f));
			}
			//////////////////////////////////
			
		mCamera->moveRelative(Ogre::Vector3(0, 0, dist));
}



bool MainClass::mouseMoved( const OIS::MouseEvent &arg )
{
	if (_gameover) {
		return true;
	}
	// Adjusts the orientation of the key movement
	if (arg.state.buttonDown(OIS::MB_Left))
	{
		camera_ePosition = eNONE;
		Ogre::Radian my = mCamera->getOrientation().getYaw();
		if (mCamera->getDirection().y < -0.999) camera_ePosition = eTop; 

		if (my >  Ogre::Radian(-0.8) && my < Ogre::Radian(0.8))
		{
			eW_direction = eNorth;
			eS_direction = eSouth;
			eA_direction = eWest;
			eD_direction = eEast;			
		}
		else if (my <  Ogre::Radian(-0.8) && my > Ogre::Radian(-2.4))
		{
			eW_direction = eEast;
			eS_direction = eWest;
			eA_direction = eNorth;
			eD_direction = eSouth;
		}
		else if (my >  Ogre::Radian(0.8) && my < Ogre::Radian(2.4))
		{
			eW_direction = eWest;
			eS_direction = eEast;
			eA_direction = eSouth;
			eD_direction = eNorth;
		}
		else if (my <  Ogre::Radian(-2.4) || my > Ogre::Radian(2.4))
		{
			eW_direction = eSouth;
			eS_direction = eNorth;
			eA_direction = eEast;
			eD_direction = eWest;
		}

		moveCamera(arg.state.X.rel, arg.state.Y.rel);


	}
	//mCameraMan->injectMouseMove(arg);

	return true;
}


bool MainClass::keyPressed( const OIS::KeyEvent &arg )
{	
	switch(arg.key)
	{
		case (OIS::KC_ESCAPE):
			mShutDown = true;
			break;

	//	case (OIS::KC_UP):
		case (OIS::KC_W):
			movementKey = OIS::KC_W;
			break;
		
	//	case (OIS::KC_LEFT):
		case (OIS::KC_A):
			movementKey = OIS::KC_A;
			break;
		
	//	case (OIS::KC_DOWN):
		case (OIS::KC_S):
			movementKey = OIS::KC_S;
			break;
		
	//	case (OIS::KC_RIGHT):
		case (OIS::KC_D):
			movementKey = OIS::KC_D;
			break;

		case (OIS::KC_SPACE):
			_player->DoAttack();
			break;
	}
	return true;
}


bool MainClass::keyReleased( const OIS::KeyEvent &arg )
{
	if (movementKey == arg.key)
			movementKey = OIS::KC_0;

	return true;
}


// Finds the next block that the player should move to, by using camera position and key direction
void MainClass::movementKeyPressed(void)
{
	Vector3 nearest = NULL_VECTOR;
	bool camIsTop = (camera_ePosition == eTop);
	switch(movementKey)
	{
		case (OIS::KC_0):
			break;	
		case (OIS::KC_W):
			if (camera_ePosition == eTop && _player->_jobQueue.empty())
			{
				nearest = FindNextPosition(eW_direction,camIsTop);
			}
			_player->Move(eW_direction, blocks_ent, camera_ePosition, nearest);
			break;	
		case (OIS::KC_S):
			if (camera_ePosition == eTop && _player->_jobQueue.empty())
			{
				nearest = FindNextPosition(eS_direction,camIsTop);
			}
			_player->Move(eS_direction, blocks_ent, camera_ePosition, nearest);
			break;			
		case (OIS::KC_A):
			if (camera_ePosition != eNONE && _player->_jobQueue.empty())
			{
				nearest = FindNextPosition(eA_direction,camIsTop);
			}
			_player->Move(eA_direction, blocks_ent, camera_ePosition, nearest);
			break;		
		case (OIS::KC_D):
			if (camera_ePosition != eNONE && _player->_jobQueue.empty())
			{
				nearest = FindNextPosition(eD_direction,camIsTop);
			}
			_player->Move(eD_direction, blocks_ent, camera_ePosition, nearest);
			break;
		default:
			break;
	}
}


Vector3 MainClass::FindNextPosition(Direction dir, bool camIsTop)
{
	if (camIsTop) return blocks_ent->getNearestBlock2(camera_ePosition, _player->abs_position, GetOrientationVector(dir, false));
	if (blocks_ent->getNearestBlock2(camera_ePosition, _player->abs_position, GetOrientationVector(dir, true)) == NULL_VECTOR)
	{		
		return blocks_ent->getNearestBlock2(camera_ePosition, _player->abs_position, GetOrientationVector(dir, false));
	}
	else return NULL_VECTOR;
}

Vector3 MainClass::GetOrientationVector(Direction dir, bool collisionCheck)
{
	Vector3 vec = Vector3(0,0,0);

	if (!collisionCheck) vec.y = -1;

	switch(dir)
	{
	case eNorth:
		vec.z = -1;
		break;
	case eSouth:
		vec.z = 1;
		break;
	case eEast:
		vec.x = 1;
		break;
	case eWest:
		vec.x = -1;
		break;
	}
	return vec;
}


