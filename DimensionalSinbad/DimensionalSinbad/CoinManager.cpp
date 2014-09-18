#include "StdAfx.h"
#include "CoinManager.h"


CoinManager::CoinManager(Ogre::SceneManager* _SceneMgr, ScoreManager* sMgr, string _fileName, Blocks* _blocks_ent)
{
	mSceneMgr = _SceneMgr;
	_scoreMgr = sMgr;
	fileName = _fileName;
	blocks_ent = _blocks_ent;
	//mPlayer = _player;
	//mPlayer->coinMgr = this;
}


CoinManager::~CoinManager(void)
{
	for (int i=0; i < coinList.size(); i++)
	{
		delete coinList[i];
	}
}


void CoinManager::Initialize()
{
	if (readLevelsFile(fileName))
	{
		GenerateCoins();
	}
}

bool CoinManager::Update(const FrameEvent& evt)
{
	return true;
}
	
void CoinManager::Destroy()
{
}


void CoinManager::GenerateCoins()
{
	Ogre::SceneNode* node;
	coinList.reserve(coinVectors.size());
	for (int i=0; i < coinVectors.size(); i++)
	{
		std::string coinName = COIN_NAME;
		std::stringstream s; 
		s << i;
		coinName.append(s.str());
		coinList.push_back(new Coin(coinName, mSceneMgr, blocks_ent->absToRealPos(coinVectors[i]), coinVectors[i]));
	}
}


bool CoinManager::CheckForCoins(Vector3 abs_position, Direction camPos)
{
	Vector3 pos;
	bool	foundCoin = false;
			cout << "campos coins" << camPos << " -> " << abs_position <<endl;
	for (int i=0; i < coinList.size(); i++)
	{
		pos = coinList[i]->absPos;
		
		switch(camPos){
			case eNorth:
			case eSouth:
				if (pos.x == abs_position.x && pos.y == abs_position.y)
					foundCoin = true;
				break;
			case eWest:
			case eEast:
				if (pos.z == abs_position.z && pos.y == abs_position.y)
					foundCoin = true;
				break;
			case eTop:
				if (pos.z == abs_position.z && pos.x == abs_position.x)
					foundCoin = true;
				break;
			case eNONE:
				if (pos == abs_position)
					foundCoin = true;
				break;
			default:
				break;
		}
		if (foundCoin)
		{
			coinList[i]->Destroy();
			delete coinList[i];
			coinList.erase(coinList.begin() + i);
			cout << "coins left" << coinList.size() << " -> " << endl;
			_scoreMgr->AddCoinScore(1);
			return true;
		}
	}
	return false;
}

bool CoinManager::readLevelsFile(string filename) {
	TiXmlDocument doc(filename.c_str());
    bool loadOkay = doc.LoadFile();

	if (loadOkay) {
		cout << "XML load succeed" << endl;
		TiXmlNode* child = 0;
		TiXmlAttribute *pAttrib = 0;

		child = doc.RootElement()->FirstChild( "Coins" );
		if (child)
			child = child ->FirstChild( "Vector" )->ToElement();

		int counter = 0;
		for( child; child; child=child->NextSiblingElement() )
		{
			counter++;
		}
		coinVectors.reserve(counter);
		cout << "CoinManager Found :" << counter << " coins" << endl;
		counter = 0;

		child = doc.RootElement()->FirstChild( "Coins" );
		if (child)
			child = child ->FirstChild( "Vector" )->ToElement();

		int x,y,z;
		for( child; child; child=child->NextSiblingElement() )
		{			
			Vector3 pos;
			pAttrib = child->ToElement()->FirstAttribute();

			if (strcmp(pAttrib->Name(), "x") == 0) {
				pos.x = atoi(pAttrib->Value());
				cout << pos.x << endl;
			} 
			pAttrib = pAttrib->Next();
			
			if (strcmp(pAttrib->Name(), "y") == 0) {
				pos.y = atoi(pAttrib->Value());
				cout << pos.y << endl;
			}
			pAttrib = pAttrib->Next();
			
			if (strcmp(pAttrib->Name(), "z") == 0) {
				pos.z = atoi(pAttrib->Value());
				cout << pos.z << endl;
			} 
			coinVectors.push_back(pos);
			counter++;
		}
		return true;
	} else {
		cout << "XML load failure" << endl;
		return false;
	}
}
