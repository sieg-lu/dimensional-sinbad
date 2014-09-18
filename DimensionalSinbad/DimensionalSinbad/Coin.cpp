#include "StdAfx.h"
#include "Coin.h"


Coin::Coin(String _name, Ogre::SceneManager* _mSceneMgr, Ogre::Vector3 _initPos, Vector3 _absPos)
{
	name = _name;
	mSceneMgr = _mSceneMgr;
	initPos = _initPos;
	absPos = _absPos;

	Initialize();
}


Coin::~Coin(void)
{
}



void Coin::Initialize()
{
	Ogre::Entity*	coin;
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	coin = mSceneMgr->createEntity(name, "knot.mesh");
	coin->setMaterialName(COIN_MATERIAL_NAME);
	coin->setCastShadows(true);
	node->attachObject(coin);
	node->setScale(COIN_SCALE, COIN_SCALE, COIN_SCALE);
	node->setPosition(initPos);
}


bool Coin::Update(const FrameEvent& evt)
{
return true;
}


void Coin::Destroy()
{
	mSceneMgr->destroySceneNode(node);
}