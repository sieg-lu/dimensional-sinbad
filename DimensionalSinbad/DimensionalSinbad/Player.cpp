#include "stdafx.h"

#include "Player.h"

Player::Player(Ogre::SceneManager* inMgr, ScoreManager* sMgr, Ogre::Vector3 initPos, Vector3 absPos)
	: _sceneMgr(inMgr), _scoreMgr(sMgr)
{
	_playerEnt = _sceneMgr->createEntity("Sinbad.mesh");
	_playerNode 
		= _sceneMgr->getRootSceneNode()->createChildSceneNode(initPos);
	_playerNode->setScale(SCALE_RATIO, SCALE_RATIO, SCALE_RATIO);
	_playerNode->attachObject(_playerEnt);

	Ogre::Entity* sword1 = _sceneMgr->createEntity("Sword1", "sword.mesh");
	Ogre::Entity* sword2 = _sceneMgr->createEntity("Sword2", "sword.mesh");
	_playerEnt->attachObjectToBone("Handle.R", sword1);
	_playerEnt->attachObjectToBone("Handle.L", sword2);

	_aniStateBottom = _playerEnt->getAnimationState("RunBase");
	_aniStateBottom->setEnabled(true);
	_aniStateBottom->setLoop(true);
	_aniStateTop = _playerEnt->getAnimationState("RunTop");
	_aniStateTop->setEnabled(true);
	_aniStateTop->setLoop(true);
	abs_position = absPos;

	_moveAnimationSpeed = 1.0f;
	_walkingSpeed = 0.05f;
	_dead = false;
	_win = false;
	coinMgr = 0;
//	Winning(10000);
}

Player::~Player()
{
}

bool Player::Initialize()
{
//	MoveForward();

//	_currentJob = Job::eIdle;
	while (!_jobQueue.empty()) {
		_jobQueue.pop();
	}
	_destination = _playerNode->getPosition();
	_step = Ogre::Vector3(0, 0, 0);
	_direction = Direction::eSouth;
	_currentAngle = 180.0f;
	_destAngle = _currentAngle;
	_isAttacking = false;

	return true;
}

bool Player::Update(const FrameEvent& evt, std::vector<Player*> enemies)
{
// 	if (DoAttackImpl(evt)) {
// 		AttackEnemy(enemy);
// 	}
	if (_win) {
		if (!WinningImpl(evt)) {
			_aniStateTop->setEnabled(false);
			_win = false;
			return false;
		}
		return true;
	}

	for (int i = 0; i < enemies.size(); i++) {
		Player* enemy = enemies[i];
		Ogre::Vector3 playerPos = this->_playerNode->getPosition();
		Ogre::Vector3 otherPos = enemy->_playerNode->getPosition();
		otherPos.y += 25;
		if (DoAttackImpl(evt)) {
			if (playerPos.positionEquals(otherPos, 20.0f)) {
				AttackEnemy(enemy);
			}
		} else if (!enemy->IsDead() && playerPos.positionEquals(otherPos, 5.0f)) {
			Killed();
		}
	}

	if (_jobQueue.empty()) {
		return false;
	}
	Job _currentJob = _jobQueue.front();
	bool res = true;
	switch (_currentJob)
	{
	case Job::eIdle:
		break;
	case Job::eMovingForward:
		if (!MoveForwardImpl(evt)) {
//			_currentJob = Job::eIdle;
			_jobQueue.pop();
			res = false;
		}
		break;
	case Job::eTurningLeft:
		if (!TurnLeftImpl(evt)) {
//			_currentJob = Job::eIdle;
			_jobQueue.pop();
			res = false;
		}
		break;
	case Job::eTurningRight:
		if (!TurnRightImpl(evt)) {
//			_currentJob = Job::eIdle;
			_jobQueue.pop();
			res = false;
		}
		break;
	case Job::eAttacking:
		break;
	case Job::eTeleporting:
		if (!Teleport(evt)) {
//			_currentJob = Job::eIdle;
			_jobQueue.pop();
			res = false;
		}
		break;
	default:
		break;
	}
	return res;
}

bool Player::Update(const FrameEvent& evt, Player* enemies)
{
	std::vector<Player*> tmp;
	tmp.push_back(enemies);
	return Update(evt, tmp);
}

bool Player::Destroy()
{
	return true;
}

void Player::MoveForward()
{
	if (shouldOverridePosition && camPos == eTop && abs_position.y < overridePos.y)
	{
		_playerNode->setPosition(blocks->absToRealPos(Vector3(abs_position.x, overridePos.y, abs_position.z)));
	}
//	_jobQueue.push(Job::eMovingForward);
// 	if (_currentJob != Job::eIdle) {
// 		return;
// 	}
//	_currentJob = Job::eMovingForward;
	_jobQueue.push(Job::eMovingForward);

//	_destination = _playerNode->getPosition() + Ogre::Vector3(0, 0, 5.0);
	switch (_direction)
	{
	case Direction::eNorth:
		{
			_destination = _playerNode->getPosition() + Ogre::Vector3(0, 0, -50.0);
			_step = Ogre::Vector3(0, 0, -50.0);
		}
		break;
	case Direction::eWest:
		{
			_destination = _playerNode->getPosition() + Ogre::Vector3(-50.0, 0, 0);
			_step = Ogre::Vector3(-50.0, 0, 0);
		}
		break;
	case Direction::eSouth:
		{
			_destination = _playerNode->getPosition() + Ogre::Vector3(0, 0, 50.0);
			_step = Ogre::Vector3(0, 0, 50.0);
		}
		break;
	case Direction::eEast:
		{
			_destination = _playerNode->getPosition() + Ogre::Vector3(50.0, 0, 0);
			_step = Ogre::Vector3(50.0, 0, 0);
		}
		break;
	default:
		break;
	}
}

bool Player::MoveForwardImpl(const FrameEvent& evt)
{
	if (_destination.positionEquals(_playerNode->getPosition(), 0.1f)) {
		_playerNode->setPosition(_destination);
// 		_aniStateBottom->setEnabled(false);
// 		_aniStateTop->setEnabled(false);
		_step = Ogre::Vector3(0, 0, 0);
		UpdateAbsPosition();
		return false;
	}
// 	std::cout << "-----\n";
// 	std::cout << _destination.x << ' ' << _playerNode->getPosition().x << std::endl;
// 	std::cout << _destination.y << ' ' << _playerNode->getPosition().y << std::endl;
// 	std::cout << _destination.z << ' ' << _playerNode->getPosition().z << std::endl;
// 	_aniStateBottom->setEnabled(true);
// 	_aniStateTop->setEnabled(true);
// 	if (_aniStateBottom->hasEnded()) {
// 		_aniStateBottom->setTimePosition(0.0f);
// 	}
// 	if (_aniStateTop->hasEnded()) {
// 		_aniStateTop->setTimePosition(0.0f);
// 	}
	_aniStateBottom->addTime(evt.timeSinceLastFrame * _moveAnimationSpeed);
	_aniStateTop->addTime(evt.timeSinceLastFrame);
	_playerNode->translate(_step /** evt.timeSinceLastFrame */* _walkingSpeed);


	return true;
}

void Player::TurnLeft()
{
// 	if (_currentJob != Job::eIdle) {
// 		return;
// 	}
// 	_currentJob = Job::eTurningRight;
	_jobQueue.push(Job::eTurningLeft);
	switch (_direction)
	{
	case Direction::eEast:
		{
			_destAngle = 90.0f;
		}
		break;
	case Direction::eNorth:
		{
			_destAngle = 0.0f;
		}
		break;
	case Direction::eWest:
		{
			_destAngle = 270.0f;
		}
		break;
	case Direction::eSouth:
		{
			_destAngle = 180.0f;
		}
		break;
	default:
		break;
	}
// 	_direction = (Direction)(((int)_direction + 1) % 4);
// 	std::cout << _direction << std::endl;
}

void Player::TurnRight()
{
// 	if (_currentJob != Job::eIdle) {
// 		return;
// 	}
// 	_currentJob = Job::eTurningRight;
	_jobQueue.push(Job::eTurningRight);
	switch (_direction)
	{
	case Direction::eEast:
		{
			_destAngle = 90.0f;
		}
		break;
	case Direction::eNorth:
		{
			_destAngle = 0.0f;
		}
		break;
	case Direction::eWest:
		{
			_destAngle = 270.0f;
		}
		break;
	case Direction::eSouth:
		{
			_destAngle = 180.0f;
		}
		break;
	default:
		break;
	}

// 	int tmp = (Direction)(((int)_direction - 1) % 4);
// 	if (tmp < 0) { tmp += 4; }
// 	_direction = (Direction)tmp;
// 	std::cout << _direction << std::endl;
}

bool Player::TurnRightImpl(const FrameEvent& evt)
{
	if (FloatEqual(_destAngle, _currentAngle, 1.0f)) {
		_currentAngle = _destAngle;
// 		_aniStateBottom->setEnabled(false);
// 		_aniStateTop->setEnabled(false);
		return false;
	}
// 	_aniStateBottom->setEnabled(true);
// 	_aniStateTop->setEnabled(true);
// 	if (_aniStateBottom->hasEnded()) {
// 		_aniStateBottom->setTimePosition(0.0f);
// 	}
// 	if (_aniStateTop->hasEnded()) {
// 		_aniStateTop->setTimePosition(0.0f);
// 	}
	_aniStateBottom->addTime(evt.timeSinceLastFrame);
	_aniStateTop->addTime(evt.timeSinceLastFrame);

	_currentAngle += ANGLE_STEP;
	if (_currentAngle >= 360.0f) {
		_currentAngle -= 360.0f;
	}
//	std::cout << _destAngle << ' ' << _currentAngle << std::endl;
	_playerNode->yaw(Ogre::Radian(Degree2Radian(-ANGLE_STEP)));

	return true;
}

bool Player::TurnLeftImpl(const FrameEvent& evt)
{
	if (FloatEqual(_destAngle, _currentAngle, 1.0f)) {
		_currentAngle = _destAngle;
// 		_aniStateBottom->setEnabled(false);
// 		_aniStateTop->setEnabled(false);
		return false;
	}
// 	_aniStateBottom->setEnabled(true);
// 	_aniStateTop->setEnabled(true);
// 	if (_aniStateBottom->hasEnded()) {
// 		_aniStateBottom->setTimePosition(0.0f);
// 	}
// 	if (_aniStateTop->hasEnded()) {
// 		_aniStateTop->setTimePosition(0.0f);
// 	}
	_aniStateBottom->addTime(evt.timeSinceLastFrame);
	_aniStateTop->addTime(evt.timeSinceLastFrame);

	_currentAngle -= ANGLE_STEP;
	if (_currentAngle < 0.0f) {
		_currentAngle += 360.0f;
	}
//	std::cout << _destAngle << ' ' << _currentAngle << std::endl;
	_playerNode->yaw(Ogre::Radian(Degree2Radian(ANGLE_STEP)));

	return true;
}

void Player::DoAttack()
{
	_aniStateTop = _playerEnt->getAnimationState("SliceVertical");
	_aniStateTop->setEnabled(true);
	_aniStateTop->setLoop(false);
	_aniStateTop->setTimePosition(0.0f);
	_isAttacking = true;
}

bool Player::DoAttackImpl(const FrameEvent& evt)
{
	if (!_isAttacking) {
		return false;
	}
//	_aniStateBottom->addTime(evt.timeSinceLastFrame);
	_aniStateTop->addTime(evt.timeSinceLastFrame * 2.0f);
	if (_aniStateTop->hasEnded()) {
		_aniStateTop->setEnabled(false);

		_aniStateTop = _playerEnt->getAnimationState("RunTop");
		_aniStateTop->setEnabled(true);
		_aniStateTop->setLoop(true);

		_isAttacking = false;

		return false;
	}
	return true;
}



// Queues the player either move forward or turn
// NOTE: Player will not attempt to move forward unless he's facing the right direction/angle
void Player::Move(Direction dir, Blocks* _blocks, Direction _camPos, Vector3 newPos)
{
	blocks = _blocks;
	camPos = _camPos;
	if (!_jobQueue.empty()) return;

	// True if the player is in 2D, this will reposition the player
	shouldOverridePosition = (newPos != NULL_VECTOR);
	overridePos = newPos  + Vector3(0,1,0);

	Vector3 pos1 = abs_position;
	Vector3 pos2 = pos1 + Vector3(0,-1,0);
	_direction = dir;
	switch (_direction)
	{
	case eNONE:
		break;
	case Direction::eNorth:
		{
			if (_currentAngle > 180) TurnRight();
			else TurnLeft();
			if (_currentAngle == 0) 
			{
				if (shouldOverridePosition  || 
					(camPos == eNONE &&
					!blocks->blockExist(pos1 + Vector3(0,0,-1)) && 
					 blocks->blockExist(pos2 + Vector3(0,0,-1))))
				{
					MoveForward();
				}
			}
		}
		break;
	case Direction::eSouth:
		{
			if (_currentAngle < 180) TurnRight();
			else TurnLeft();
			if (_currentAngle == 180) 
			{
				if (shouldOverridePosition  || 
					(camPos == eNONE &&
					!blocks->blockExist(pos1 + Vector3(0,0,1)) &&
					 blocks->blockExist(pos2 + Vector3(0,0,1))))
				{
					MoveForward();
				}
			}
		}
		break;
	case Direction::eWest:
		{
			if (_currentAngle < 270 && _currentAngle > 90) TurnRight();
			else TurnLeft();
			if (_currentAngle == 270) 
			{
				if (shouldOverridePosition  || 
					(camPos == eNONE &&
					!blocks->blockExist(pos1 + Vector3(-1,0,0)) &&
					 blocks->blockExist(pos2 + Vector3(-1,0,0))))
				{
					MoveForward();
				}
			}
		}
		break;
	case Direction::eEast:
		{
			if (_currentAngle <= 270 && _currentAngle < 90) TurnRight();
			else TurnLeft();
			if (_currentAngle == 90) 
			{
				if (shouldOverridePosition  || 
					(camPos == eNONE &&
					!blocks->blockExist(pos1 + Vector3(1,0,0)) && 
					 blocks->blockExist(pos2 + Vector3(1,0,0))))
				{
					MoveForward();
				}
			}
		}
		break;
	default:
		break;
	}



}


// This should be called at the end of MoveForwardImpl to increment the abstract player position
// or re-adjust, if the player got repositioned (This occurs only in 2D)
void Player::UpdateAbsPosition()
{
	
	if (shouldOverridePosition)
	{
		abs_position = overridePos;
		_playerNode->setPosition(blocks->absToRealPos(abs_position));	
		if (coinMgr != 0) coinMgr->CheckForCoins(abs_position, camPos);
		return;
	}


	switch (_direction)
	{
	case Direction::eNorth:
		{
			abs_position += Vector3(0,0,-1);
		}
		break;
	case Direction::eSouth:
		{
			abs_position += Vector3(0,0,1);
		}
		break;
	case Direction::eWest:
		{
			abs_position += Vector3(-1,0,0);
		}
		break;
	case Direction::eEast:
		{
			abs_position += Vector3(1,0,0);
		}
		break;
	default:
		break;
	}
	if (coinMgr != 0) coinMgr->CheckForCoins(abs_position, camPos);
}

bool Player::AttackEnemy(Player* other)
{
	other->Killed();
	_scoreMgr->AddEnemyScore(1);

//	std::cout << "Player Meet.\n";
	return true;
}

void Player::Killed()
{
	// TO DO: Game Over Event.
	_dead = true;
	std::cout << "Killed.\n";
//	throw std::string("TO DO: Game Over");
}

void Player::Winning(int danceTimes)
{
	_win = true;
	_tgtTime = danceTimes;
	_aniStateTop = _playerEnt->getAnimationState("Dance");
	_aniStateTop->setEnabled(true);
	_aniStateTop->setLoop(true);
	_aniStateTop->setTimePosition(0.0f);
	_winningTimer.Start();
}

bool Player::WinningImpl(const FrameEvent& evt)
{
	_aniStateTop->addTime(evt.timeSinceLastFrame);
	std::cout << _winningTimer.CurrentTime() << std::endl;
	if (_winningTimer.CurrentTime() > _tgtTime) {
		return false;
	}
// 	if (_aniStateTop->hasEnded()) {
// 		_aniStateTop->setEnabled(false);
// 
// 		return false;
// 	}
	return true;
}


// Queues the player to be repositioned (teleported) after a 3D to 2D switch
void Player::DimensionalUpdate(Direction _camPos, Blocks* _blocks)
{	
	camPos = _camPos;
	blocks = _blocks;
	if (coinMgr != 0) coinMgr->CheckForCoins(abs_position, camPos);
	_jobQueue.emplace(Job::eTeleporting);
}

// Moves the player to a new location according to the camera position
bool Player::Teleport(const FrameEvent& evt)
{
	Vector3 pos1 = abs_position;
	Vector3 pos2 = pos1 + Vector3(0,-1,0);
	Vector3 orient;
	Vector3 newPos;

	switch (camPos)
	{
	case Direction::eNorth:
		{
			orient = Vector3(0,0,-1);
		}
		break;
	case Direction::eSouth:
		{
			orient = Vector3(0,0,1);
		}
		break;
	case Direction::eWest:
		{
			orient = Vector3(1,0,0);
		}
		break;
	case Direction::eEast:
		{
			orient = Vector3(-1,0,0);
		}
		break;
	default:
		return false;
		break;
	}

	if (blocks->getNearestBlock2(camPos, pos1 ,orient) == NULL_VECTOR)
	{ 
		newPos = blocks->getNearestBlock2(camPos, pos2 ,orient);
		if (newPos != NULL_VECTOR && newPos != abs_position + Vector3(0,1,0)){
			abs_position = newPos + Vector3(0,1,0);
			_playerNode->setPosition(blocks->absToRealPos(abs_position));	
		}
	}

	return false;
}

Vector3 Player::getPlayerPosition() {
	return _playerNode->getPosition();
}

