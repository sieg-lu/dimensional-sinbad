#include "stdafx.h"

#include "TinyXml/tinyxml.h"
#include "EnemyManager.h"



EnemyManager::EnemyManager(char* filename, Ogre::SceneManager* _inMgr, Blocks* block_ent)
{
	vData.clear();
	vEnemy.clear();
	ReadXmlEnemyData(filename);

	for (int i = 0; i < vData.size(); i++) {
		Ogre::Vector3 startPos = Ogre::Vector3(vData[i].x, vData[i].y, vData[i].z);
		Ogre::Vector3 pos = block_ent->absToRealPos(startPos);
		Enemy* enemy = new Enemy(_inMgr, Ogre::Vector3(pos.x, pos.y-25, pos.z), startPos);
		vEnemy.push_back(enemy);
		enemy->Initialize();
		enemy->InitCmdVector(vData[i].vCmd);
	}
}

EnemyManager::~EnemyManager()
{
	for (int i = 0; i < vEnemy.size(); i++) {
		if (vEnemy[i]) {
			delete vEnemy[i];
		}
	}
//	vEnemy.clear();
}

bool EnemyManager::Initialize()
{
// 	for (int i = 0; i < vEnemy.size(); i++) {
// 		vEnemy[i]->Initialize();
// 	}
	return true;
}

bool EnemyManager::Update(const FrameEvent& evt, Player* pl)
{
// 	Ogre::Vector3 t1, t2;
// 	t2 = pl->_playerNode->getPosition();
// 	std::cout << "Player: " << t2.x << ' ' << t2.y << ' ' << t2.z << std::endl;
	for (int i = 0; i < vEnemy.size(); i++) {
// 		t1 = vEnemy[i]->_playerNode->getPosition();
// 		std::cout << "Enemy" << i << ": " << t1.x << ' ' << t1.y << ' ' << t1.z << std::endl;
		vEnemy[i]->Update(evt, pl);
	}
	return true;
}

bool EnemyManager::Destroy()
{
	for (int i = 0; i < vEnemy.size(); i++) {
		vEnemy[i]->Destroy();
	}
	return true;
}

void EnemyManager::ReadXmlEnemyData(const char* filename)
{
	TiXmlDocument doc;
	if (doc.LoadFile(filename)) {
		doc.Print();
	} else {
		MyLog::put(__LINE__, __FILE__, "can not parse xml file.\n");
		return;
	}

	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* keyElement = rootElement->FirstChildElement();

	while (keyElement) {
		EnemyData ed;
		TiXmlElement* ele = keyElement->FirstChildElement();
		TiXmlAttribute* coord = ele->FirstAttribute();
		ed.x = atoi(coord->Value());
		coord = coord->Next();
		ed.y = atoi(coord->Value());
		coord = coord->Next();
		ed.z = atoi(coord->Value());

		ele = ele->NextSiblingElement();
		TiXmlElement* cmdEle = ele->FirstChildElement();
		while (cmdEle) {
			TiXmlAttribute* cmdName = cmdEle->FirstAttribute();
			if (strcmp(cmdName->Value(), "MovingForward") == 0) {
				ed.vCmd.push_back(Job::eMovingForward);
			} else if (strcmp(cmdName->Value(), "TurningLeft") == 0) {
				ed.vCmd.push_back(Job::eTurningLeft);
			} else if (strcmp(cmdName->Value(), "TurningRight") == 0) {
				ed.vCmd.push_back(Job::eTurningRight);
			}
			cmdEle = cmdEle->NextSiblingElement();
		}

		vData.push_back(ed);
		keyElement = keyElement->NextSiblingElement();
	}
}
