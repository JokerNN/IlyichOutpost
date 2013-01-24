#pragma once
#include "cocos2d.h"

#include "SimpleAudioEngine.h"

#include "Waypoint.h"
#include "Wave.h"
#include "Tower.h"
#include "TowerArray.h"

#include <set>

typedef vector<Tower>::iterator towerArrayIterator;

class MainFieldScene : public cocos2d::CCLayer
{
	Waypoint way;
	Wave *wave;

	int wavesCount;
	int waveTimout;
	TowerArray towers;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(MainFieldScene);

	// Menu items handlers
	void CreateScene(CCObject* sender);		

	void StartWave(float dt);
	void WaveGenerateEnemyProcess(float dt);
	void GameLogic(float dt);
	Tower addTower(int towerType, cocos2d::CCPoint position);
};
