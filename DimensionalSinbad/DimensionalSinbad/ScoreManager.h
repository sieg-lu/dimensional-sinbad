#ifndef __SCORE_MANAGER_H__
#define __SCORE_MANAGER_H__

#include <iostream>
#include "Utility.h"
using namespace std;

class ScoreManager {

private:
	int							_score;
	int							_coinScore;
	int							_enemyScore;
	CustomTimer					_timer;

public:
	ScoreManager(int coin, int enemy)
		: _score(0)
		, _coinScore(coin)
		, _enemyScore(enemy) { }

	void						GameStart()
	{
		_timer.Start();
	}

	void						AddCoinScore(int times)
	{
		_score += times*(_coinScore/_timer.CurrentTime());
	}

	void						AddEnemyScore(int times)
	{
		_score += times*(_enemyScore/_timer.CurrentTime());
	}

	int							getScore() const				{ return _score; }
	int							getCurrentTime()				{ return _timer.CurrentTime(); }
};



#endif ///> end of __SCORE_MANAGER_H__