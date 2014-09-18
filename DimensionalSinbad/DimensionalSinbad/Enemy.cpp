#include "stdafx.h"

#include "Enemy.h"


Enemy::Enemy(Ogre::SceneManager* inMgr, Ogre::Vector3 initPos, Vector3 absPos)
{
	_sceneMgr = inMgr;
	
	_playerEnt = _sceneMgr->createEntity("ninja.mesh");
	_playerNode 
		= _sceneMgr->getRootSceneNode()->createChildSceneNode(initPos);
	_playerNode->setScale(ENEMY_SCALE_RATIO, ENEMY_SCALE_RATIO, ENEMY_SCALE_RATIO);
	_playerNode->attachObject(_playerEnt);
	_playerNode->yaw(Ogre::Radian(Ogre::Math::PI));

	_aniStateTop = _playerEnt->getAnimationState("Attack1");
	_aniStateTop->setEnabled(true);
	_aniStateTop->setLoop(true);

	_aniStateBottom = _playerEnt->getAnimationState("Walk");
	_aniStateBottom->setEnabled(true);
	_aniStateBottom->setLoop(true);
	abs_position = absPos;

	_moveAnimationSpeed = 5.0f;
	_walkingSpeed = 0.02f;
	_dead = false;
	_cmdIndex = 0;
	run = false;

}

Enemy::~Enemy()
{
	Player::~Player();
}

bool Enemy::Initialize()
{
	Player::Initialize();
	run = true;
//	InitCmdVector();
	return true;
}

bool Enemy::Update(const FrameEvent& evt, Player* enemy)
{
	if (!run) return true;
	if (_dead) {
		KilledImpl(evt);
		return true;
	}
// 	std::cout << "Player: ";
// 	std::cout << _playerNode->getPosition().x << ' ';
// 	std::cout << _playerNode->getPosition().y << ' ';
// 	std::cout << _playerNode->getPosition().z << '\n';
// 	std::cout << "Enemy: ";
// 	std::cout << enemy->_playerNode->getPosition().x << ' ';
// 	std::cout << enemy->_playerNode->getPosition().y << ' ';
// 	std::cout << enemy->_playerNode->getPosition().z << '\n';

// 	std::cout << _direction << ' ';
// 	std::cout << enemy->_direction << '\n';

	if (!Player::Update(evt, enemy)) {
		NextCmd();
	}
	return true;
}

bool Enemy::Destroy()
{
	Player::Destroy();
	return true;
}

void Enemy::DoAttack()
{
	_aniStateTop = _playerEnt->getAnimationState("Attack1");
	_aniStateTop->setEnabled(true);
	_aniStateTop->setLoop(false);
	_aniStateTop->setTimePosition(0.0f);
	_isAttacking = true;
}

bool Enemy::DoAttackImpl(const FrameEvent& evt)
{
	if (!_isAttacking) {
		return false;
	}
	//	_aniStateBottom->addTime(evt.timeSinceLastFrame);
	_aniStateTop->addTime(evt.timeSinceLastFrame * 2.0f);
	if (_aniStateTop->hasEnded()) {
		_aniStateTop->setEnabled(false);

		_aniStateTop = _playerEnt->getAnimationState("Attack1");
		_aniStateTop->setEnabled(true);
		_aniStateTop->setLoop(true);

		_isAttacking = false;

		return false;
	}
	return true;
}

void Enemy::InitCmdVector()
{
	_cmdVector.clear();

// 	_cmdVector.push_back(Job::eMovingForward);
// 	_cmdVector.push_back(Job::eTurningRight);
// 	_cmdVector.push_back(Job::eMovingForward);
// 	_cmdVector.push_back(Job::eTurningRight);
// 	_cmdVector.push_back(Job::eMovingForward);
// 	_cmdVector.push_back(Job::eTurningRight);
// 	_cmdVector.push_back(Job::eMovingForward);
// 	_cmdVector.push_back(Job::eTurningRight);

	_cmdVector.push_back(Job::eTurningLeft);
	_cmdVector.push_back(Job::eTurningLeft);
	_cmdVector.push_back(Job::eMovingForward);
	_cmdVector.push_back(Job::eTurningRight);
	_cmdVector.push_back(Job::eMovingForward);
	_cmdVector.push_back(Job::eMovingForward);
	_cmdVector.push_back(Job::eMovingForward);
	_cmdVector.push_back(Job::eMovingForward);
	_cmdVector.push_back(Job::eTurningLeft);
	_cmdVector.push_back(Job::eTurningLeft);
	_cmdVector.push_back(Job::eMovingForward);
	_cmdVector.push_back(Job::eMovingForward);
	_cmdVector.push_back(Job::eMovingForward);
	_cmdVector.push_back(Job::eMovingForward);
	_cmdVector.push_back(Job::eTurningLeft);
	_cmdVector.push_back(Job::eMovingForward);
}

void Enemy::InitCmdVector(std::vector<Job> vv)
{
	_cmdVector.clear();
	for (int i = 0; i < vv.size(); i++) {
		_cmdVector.push_back(vv[i]);
	}
}

void Enemy::NextCmd()
{
	switch (_cmdVector[_cmdIndex])
	{
	case Job::eMovingForward:
		{
			MoveForward();
		}
		break;
	case Job::eTurningLeft:
		{
			TurnLeft();
//			std::cout << "left\n";
		}
		break;
	case Job::eTurningRight:
		{
			TurnRight();
//			std::cout << "right\n";
		}
		break;
	default:
		break;
	}
	_cmdIndex++;
	if (_cmdIndex >= _cmdVector.size()) {
		_cmdIndex = 0;
	}
}

void Enemy::TurnRight()
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
			_destAngle = 180.0f;
		}
		break;
	case Direction::eNorth:
		{
			_destAngle = 90.0f;
		}
		break;
	case Direction::eWest:
		{
			_destAngle = 0.0f;
		}
		break;
	case Direction::eSouth:
		{
			_destAngle = 270.0f;
		}
		break;
	default:
		break;
	}
	int tmp = (((int)_direction - 1) % 4);
	if (tmp < 0) {
		tmp += 4;
	}
	_direction = (Direction)tmp;
//	std::cout << _direction << std::endl;
}

void Enemy::TurnLeft()
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
			_destAngle = 0.0f;
		}
		break;
	case Direction::eNorth:
		{
			_destAngle = 270.0f;
		}
		break;
	case Direction::eWest:
		{
			_destAngle = 180.0f;
		}
		break;
	case Direction::eSouth:
		{
			_destAngle = 90.0f;
		}
		break;
	default:
		break;
	}
	_direction = (Direction)(((int)_direction + 1) % 4);
//	std::cout << _direction << std::endl;
}

bool Enemy::AttackEnemy(Player* other)
{
	Ogre::Vector3 playerPos = this->_playerNode->getPosition();
	Ogre::Vector3 otherPos = other->_playerNode->getPosition();
	otherPos.y -= 20;
	if (!playerPos.positionEquals(otherPos, 1.0f)) {
		return false;
	}
	std::cout << "Enemy Meet.\n";

	return true;
}

void Enemy::Killed()
{
	_dead = true;
	_aniStateTop = _playerEnt->getAnimationState("Death1");
	_aniStateTop->setEnabled(true);
	_aniStateTop->setLoop(false);
	_aniStateTop->setTimePosition(0.0f);
	_isAttacking = true;
}

bool Enemy::KilledImpl(const FrameEvent& evt)
{
	if (!_dead) {
		return false;
	}
	//	_aniStateBottom->addTime(evt.timeSinceLastFrame);
	_aniStateTop->addTime(evt.timeSinceLastFrame * 2.0f);
	if (_aniStateTop->hasEnded()) {
		_aniStateTop->setEnabled(false);

		_playerEnt->setVisible(false);

// 		_sceneMgr->destroyEntity(_playerEnt);
// 		_sceneMgr->destroySceneNode(_playerNode);

		return false;
	}
	return true;
}

bool Enemy::MoveForwardImpl(const FrameEvent& evt)
{
	if (_destination.positionEquals(_playerNode->getPosition(), 0.1f)) {
		_playerNode->setPosition(_destination);

		_step = Ogre::Vector3(0, 0, 0);
		return false;
	}

	_aniStateBottom->addTime(evt.timeSinceLastFrame * _moveAnimationSpeed);
	_aniStateTop->addTime(evt.timeSinceLastFrame);
	_playerNode->translate(_step /** evt.timeSinceLastFrame */* _walkingSpeed);


	return true;
}
