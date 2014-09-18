/*
  Blocks.h
  Author: Xiaoxuan Zhang
  Description:
  This class implement the presentation of three types of blocks.
  It uses abstract position to represent the center coordinates of a block.

*/

#pragma once
#include "stdafx.h"
#include "Constants.h"

using namespace Ogre;
using namespace std;

class Blocks {
public :
	Blocks(string filename, SceneManager *inputSceneMgr);
	~Blocks();

	static std::vector<String> readLevelsFile(string filename) {
		std::vector<String> fileNames;
		TiXmlDocument doc(filename.c_str());
		bool loadOkay = doc.LoadFile();

		if (loadOkay) {
			TiXmlNode* node = 0;
			TiXmlNode* root = 0;
			TiXmlAttribute *pAttrib = 0;
			string test;
			root = doc.RootElement();
			node = root->FirstChild();
			while (node) {
				test = node->Value();
				pAttrib = node->ToElement()->FirstAttribute();
				test = pAttrib->Value();
				fileNames.push_back(pAttrib->Value());
				node = node->NextSibling();
			}
		}
		return fileNames;
	}

	static Vector3 getPlayerStartPos(string filename) {
		Vector3 startPosition = Vector3(4,1,0);
		TiXmlDocument doc(filename.c_str());
		bool loadOkay = doc.LoadFile();

		if (loadOkay) {
			TiXmlNode* child = 0;
			TiXmlAttribute *pAttrib = 0;
			child = doc.RootElement()->FirstChild( "PlayerPosition" );
			if (child) {
				child = child ->FirstChild( "Vector" )->ToElement();
			}

			pAttrib = child->ToElement()->FirstAttribute();

			if (strcmp(pAttrib->Name(), "x") == 0) {
				startPosition.x = atoi(pAttrib->Value());
				cout << startPosition.x << endl;
			} 
			pAttrib = pAttrib->Next();
			
			if (strcmp(pAttrib->Name(), "y") == 0) {
				startPosition.y = atoi(pAttrib->Value());
				cout << startPosition.y << endl;
			}
			pAttrib = pAttrib->Next();
			
			if (strcmp(pAttrib->Name(), "z") == 0) {
				startPosition.z = atoi(pAttrib->Value());
				cout << startPosition.z << endl;	
			}
		}
		return startPosition;
	}

	bool blockExist(Vector3 pos);
	Vector3 getNearestBlock2(Direction camPos, Vector3 pos, Vector3 dir);
	Vector3 absToRealPos(Vector3 absPos);
	Vector3 Blocks::realToAbsPos(Vector3 realPos);
	int isWin(Direction camPos, Vector3 playerPos);
private:
	SceneNode** nodes;
	int NODENUM;
	SceneManager *sceneMgr;
	int blockCounter;
	Real CUBESCALE;
	Vector3 startCenter;
	Real SIDE;
	int WIDTH;
	int LENGTH;
	int HEIGHT;
	int ***nodesArr;

	void setPosToArr(Vector3 pos, enum blockColor color);

	SceneNode* createChildAndAttach(enum blockColor color);
	Entity* createCube(int num, enum blockColor color);
	std::string getMaterial(enum blockColor color);

	void readConfigFile(string filename);
	bool readLevelFile(string filename, std::vector<Vector3> &whiteVectors, 
		std::vector<Vector3> &redVectors, std::vector<Vector3> &greenVectors, Real &side, int &width, int &length, int &height, Real &scale);
	std::vector<Vector3> getAllVectors(TiXmlNode* node);
	void getWorldSize(TiXmlNode* node, Real &side, int &width, int &length, int &height, Real &scale);
	void generateNodes(std::vector<Vector3> vectors, enum blockColor color, int &counter);
};
