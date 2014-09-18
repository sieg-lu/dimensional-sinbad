
#include "stdafx.h"
#include "Blocks.h"
// constructor
// inputSceneMgr used to create blocks
// create a 3 dimentional array to store the block information
// read the block position information form XML file
// create a scenenode for each block
Blocks::Blocks(string filename, SceneManager *inputSceneMgr) {
	std::vector<Vector3> whiteVectors, redVectors, greenVectors;
	readLevelFile(filename, whiteVectors, redVectors, greenVectors, SIDE, LENGTH, WIDTH, HEIGHT, CUBESCALE);

	startCenter.x = -0.5 * LENGTH * SIDE + 0.5 * SIDE;
	startCenter.z = -0.5 * WIDTH * SIDE + 0.5 * SIDE; ; 
	startCenter.y = 0.5 * SIDE;  
	sceneMgr = inputSceneMgr;
	int blockCounter = 0;
	

		// update the abstract array
	nodesArr = new int**[LENGTH];
	for (int i = 0; i < LENGTH; i++) {
		nodesArr[i] = new int*[HEIGHT];
		for (int j = 0; j < HEIGHT; j++) {
			nodesArr[i][j] = new int[WIDTH];
		}
	}

	for (int i = 0; i < LENGTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {\
			for (int k = 0; k < WIDTH; k++) {
				nodesArr[i][j][k] = 0;
			}
		}
	}
	
	int whiteVectorSize = whiteVectors.size();
	int greenVectorSize = greenVectors.size();
	int redVectorSize = redVectors.size();
	NODENUM = whiteVectorSize + greenVectorSize + redVectorSize;
	nodes = new SceneNode*[NODENUM];
	int counter = 0;
	generateNodes(whiteVectors, BLOCK_COLOR_WHITE, counter);
	generateNodes(greenVectors, BLOCK_COLOR_GREEN, counter);
	generateNodes(redVectors, BLOCK_COLOR_RED, counter);
}

// create scenenodes according to the given vector and color				
void Blocks::generateNodes(std::vector<Vector3> vectors, enum blockColor color, int &counter) {
	std::vector<Vector3>::const_iterator ciVector;
	for(ciVector=vectors.begin(); ciVector!=vectors.end(); ciVector++, counter++) {
		nodes[counter] = createChildAndAttach(color);
		nodes[counter]->setPosition(absToRealPos(*ciVector));
		setPosToArr(*ciVector, color);
	}
}

// destructor
// delete all the scenenodes 
// delete the 3 dimensitional array
Blocks::~Blocks()
{
	// remove all nodes
	for (int i = 0; i < NODENUM; i++) {
		nodes[i]->getParent()->removeChild(nodes[i]);
	}

	for (int i = 0; i < LENGTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			delete[] nodesArr[i][j];
		}
	}

}

// read level data from a XML file 
// store data into vectors and variables
bool Blocks::readLevelFile(string filename, std::vector<Vector3> &whiteVectors, 
	std::vector<Vector3> &redVectors, std::vector<Vector3> &greenVectors, Real &side, int &width, int &length, int &height, Real &scale) {
	TiXmlDocument doc(filename.c_str());
    bool loadOkay = doc.LoadFile();

	if (loadOkay) {
		cout << "XML load succeed" << endl;
		TiXmlNode* node = 0;
		TiXmlNode* root = 0;
		TiXmlNode* vectors = 0;
		string test;
        root = doc.RootElement();
        
		vectors = root->FirstChild();
		getWorldSize(vectors, side, width, length, height, scale);


		vectors = vectors->NextSibling();
		node = vectors->FirstChild();
		whiteVectors = getAllVectors(node); 

		vectors = vectors->NextSibling();
		node = vectors->FirstChild();
		redVectors = getAllVectors(node); 

		vectors = vectors->NextSibling();
		node = vectors->FirstChild();
		greenVectors = getAllVectors(node); 

		return true;
	} else {
		cout << "XML load filure" << endl;
		return false;
	}
}

// read all vector node within a given XML node into a vector of Vector3
std::vector<Vector3> Blocks::getAllVectors(TiXmlNode* node) {
	std::vector<Vector3> vectors; 
	if (node) {
		TiXmlAttribute *pAttrib = node->ToElement()->FirstAttribute();
	
		int x, y, z;	
		while (pAttrib && node) {
			if (strcmp(pAttrib->Name(), "x") == 0) {
				x = atoi(pAttrib->Value());
				cout << x << endl;
			} 
			pAttrib=pAttrib->Next();
			if (pAttrib) {
				if (strcmp(pAttrib->Name(), "y") == 0) {
					y = atoi(pAttrib->Value());
					cout << y << endl;
				} 
			} 
			pAttrib=pAttrib->Next();
			if (pAttrib) {
				if (strcmp(pAttrib->Name(), "z") == 0) {
					z = atoi(pAttrib->Value());
					cout << z << endl;
				} 
			}
			vectors.push_back(Vector3(x, y, z));
			node = node->NextSibling();
			if (node) {
				pAttrib = node->ToElement()->FirstAttribute();
			}
		}
	}
	return vectors;
}

// convert the abstract position into the real positon accoring to the SIDE, LENGTH WIDTH, HEIGHT
Vector3 Blocks::absToRealPos(Vector3 absPos) {
	return Vector3(absPos.x * SIDE + startCenter.x, absPos.y * SIDE + startCenter.y, absPos.z * SIDE + startCenter.z);
}

// get the nearest block from the camera, according to the given block and the orent
// curPos is the abstract position of the current block
// outputVector is the abstraction position of the nearest block found
// orient is the searching orientation, the orient vector should like this (1, 0, 0), (0, -1, 0)
// either x, y, z value should be 1 or -1
// if the block is found, return true, otherwise return false
// return the nearest block if exist, return NULL_VECTOR if doesn't
Vector3 Blocks::getNearestBlock2(Direction camPos, Vector3 pos, Vector3 dir)
{
	Vector3 nearest = NULL_VECTOR;
	int startCoord = 0;	
	int xPos = pos.x + dir.x;
	int yPos = pos.y + dir.y;
	int zPos = pos.z + dir.z;
	if ((xPos < 0) ||
		(yPos < 0) ||
		(zPos < 0) ||
		(xPos >= LENGTH) ||
		(yPos >= HEIGHT) ||
		(zPos >= WIDTH))
		return nearest;

	switch(camPos){
		case eNorth:
			{
				if (dir.x != 0)
					startCoord = WIDTH-1;
				else if (dir.z != 0)
					startCoord = pos.z;

				for (int vz = startCoord; vz >= 0; vz--)
				{
					if (nodesArr[xPos][yPos][vz] != NULL)
						nearest = Vector3(xPos, yPos, vz);
				}

			}
			break;
		case eSouth:
			{
				if (dir.x != 0)
					startCoord = 0;
				else if (dir.z != 0)
					startCoord = pos.z;

				for (int vz = startCoord; vz < WIDTH; vz++)
				{
					if (nodesArr[xPos][yPos][vz] != NULL)
						nearest = Vector3(xPos, yPos, vz);
				}
			}
			break;
		case eWest:
			{
				if (dir.x != 0)
					startCoord = pos.x;
				else if (dir.z != 0)
					startCoord = 0;

				for (int vx = startCoord; vx < LENGTH; vx++)
				{
					if (nodesArr[vx][yPos][zPos] != NULL)
						nearest = Vector3(vx, yPos, zPos);
				}
			}
			break;
		case eEast:
			{
				if (dir.x != 0)
					startCoord = pos.x;
				else if (dir.z != 0)
					startCoord = LENGTH-1;

				for (int vx = startCoord; vx >= 0; vx--)
				{
					if (nodesArr[vx][yPos][zPos] != NULL)
						nearest = Vector3(vx, yPos, zPos);
				}
			}
			break;
		case eTop:
			{
				if (dir.y != 0)
					startCoord = 0;

				for (int vy = startCoord; vy < HEIGHT; vy++)
				{
					if (nodesArr[xPos][vy][zPos] != NULL)
						nearest = Vector3(xPos, vy, zPos);
				}
			}
			break;

	}
	//std::cout<< "\ngiven  " << camPos << "  " << pos << "   " << dir;
	//std::cout<< "\nreturning  " << nearest;
	return nearest;
	
}

/// Returns True if a block exists in that position
bool Blocks::blockExist(Vector3 pos)
{
	int x,y,z;
	x = pos.x;
	y = pos.y;
	z = pos.z;
	if (x < 0 || y < 0 || z < 0 ||
		x >= LENGTH || y >= HEIGHT || z >= WIDTH)
		return false;
	else
		return (nodesArr[x][y][z] != 0);
}

// using the given positon set the corresponding element of array with the block color
void Blocks::setPosToArr(Vector3 pos, enum blockColor color) {
	int x = (int)pos.x;
	int y = (int)pos.y;
	int z = (int)pos.z;
	nodesArr[x][y][z] = color;
}

// create a scenenode of a block
SceneNode* Blocks::createChildAndAttach(enum blockColor color) {
	Ogre::SceneNode* mNewCubeNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	mNewCubeNode->attachObject(createCube(blockCounter, color));
	mNewCubeNode->setScale(Ogre::Vector3(CUBESCALE, CUBESCALE, CUBESCALE));
	blockCounter++;
	return mNewCubeNode;
}

// create a entity of cube
Entity* Blocks::createCube(int num, enum blockColor color) {
	std::string entName = "Cube";
	std::stringstream s;
    s << num;
	entName.append(s.str());
	Ogre::Entity* newEnt = sceneMgr->createEntity(entName, "cube.mesh");
		
	std::string materiaName;
	materiaName = std::string(getMaterial(color));

	newEnt->setMaterialName(materiaName);
	newEnt->setCastShadows(true);
	return newEnt;
}

// the the material name according to the given block color
std::string Blocks::getMaterial(enum blockColor color) {
	switch (color)
	{
	case BLOCK_COLOR_GREEN:
		return GREEN_BLOCK_MATERIAL;
	case BLOCK_COLOR_RED:
		return RED_BLOCK_MATERIAL;
	default :
		return WHITE_BLOCK_MATERIAL;
	}
}

// check whether the player win or lose the game
// 0 not win, 1 win, 2 lose
int Blocks::isWin(Direction camPos, Vector3 playerPos) {
	Vector3 blockPos;
	Vector3 absPlayerPos = realToAbsPos(playerPos);
	if (camPos == eNONE) {
		// 3D
		blockPos = absPlayerPos + Vector3(0, -1, 0);
	} else {
		// 2D
		blockPos = getNearestBlock2(camPos, absPlayerPos, Vector3(0, -1, 0));
	}

	int ret = 0;
	if (blockPos != NULL_VECTOR) {
		int x, y, z;
		x = blockPos.x;
		y = blockPos.y;
		z = blockPos.z;
		if (nodesArr[x][y][z] != 0) {
			if (nodesArr[x][y][z] == BLOCK_COLOR_GREEN) {
				ret = 1;		
			} else if (nodesArr[x][y][z] == BLOCK_COLOR_RED) {
				ret = 2;
			}
		}
	}
	return ret;
}

// convert the real coordinates into abstraction coordinates
Vector3 Blocks::realToAbsPos(Vector3 realPos) {
	return Vector3((realPos.x - startCenter.x) / SIDE, (realPos.y - startCenter.y) / SIDE, (realPos.z - startCenter.z) / SIDE);
}

// read side, length, width, height and scale from XML file
void Blocks::getWorldSize(TiXmlNode* node, Real &side, int &length, int &width, int &height, Real &scale) {
	std::string test = node->Value();
	TiXmlAttribute *pAttrib = node->ToElement()->FirstAttribute();
	if (strcmp(pAttrib->Name(), "side") == 0) {
		side = atof(pAttrib->Value());
		cout << "side" << side << endl;
	} 
	pAttrib=pAttrib->Next();
	if (pAttrib) {
		if (strcmp(pAttrib->Name(), "length") == 0) {
			length = atoi(pAttrib->Value());
			cout << "length" << length << endl;
		} 
	} 
	pAttrib=pAttrib->Next();
	if (pAttrib) {
		if (strcmp(pAttrib->Name(), "width") == 0) {
			width = atoi(pAttrib->Value());
			cout << "width" << width << endl;
		} 
	}
	pAttrib=pAttrib->Next();
	if (pAttrib) {
		if (strcmp(pAttrib->Name(), "height") == 0) {
			height = atoi(pAttrib->Value());
			cout << "height" << height << endl;
		} 
	}

	pAttrib=pAttrib->Next();
	if (pAttrib) {
		if (strcmp(pAttrib->Name(), "scale") == 0) {
			scale = atof(pAttrib->Value());
			cout << "scale" << scale << endl;
		} 
	}
}