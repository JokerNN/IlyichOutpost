#pragma once
#include "cocos2d.h"
#include "Wave.h"
#include <math.h>
#include "Wave.h"
#include "Enemy.h"
#include "UpgradeBase.h"
#include <cstdlib>

#define GAME_SPEED 10

#define UPGRADES_COUNT_TOWER 5
#define UPGRADE_COEF_TOWER 1.2

enum TowerTypes{
	MACHINE_GUN = 1,
	HEAVY_GUN = 2,
	SNIPER_GUN,
	ANTITANK_GUN,
	LASER_GUN,
	ROCKET_GUN,
	ELECTROMAGNETIC_GUN,
	FLAME_GUN,
	NAPALM_GUN
};

enum CircleHalf{  //for turns processing
	UPPER_HALF = 0,
	LOWER_HALF
};

class Tower : public UpgradeBase, public cocos2d::CCObject
{
protected:
	int fireSpeed;
	int fireRadius;
	int damage;
	int reloadTime;
	float normRotateX;
	float normRotateY;
	TowerTypes type;

	int index;
	
	cocos2d::CCPoint position;
	cocos2d::CCSprite *spritePtr;
	cocos2d::CCSprite *spriteRangePtr;
	cocos2d::CCSprite *firingSprite;

	void fire(Wave *wave, int index);
	void startFireAnimation(const cocos2d::CCPoint startPosition, const cocos2d::CCPoint endPosistion);
	void deleteShell(cocos2d::CCNode *sender);
	void endFireAnimation(cocos2d::CCNode *sender);
	Wave *targets;

public:
	Tower(MoneyManager *moneyManager, TowerTypes type, cocos2d::CCPoint _position);
	~Tower(void);
	const float getX() const { return position.x;};
	const float getY() const { return position.y;};
	void setX(const float newX);
	void setY(const float newY);
	const float getDamage() const { return damage;};
	cocos2d::CCSprite *getSprite() const;
	void turnTo(const cocos2d::CCPoint position) const;
	const bool operator < (const Tower &tower) const;
	const bool operator > (const Tower &tower) const;
	const bool isTargetInRange(cocos2d::CCPoint target) const;
	virtual void processEnemies(Wave *wave);
	const bool isAbleToFire() const;
	TowerTypes GetType() const;
	int GetSellPrice() const;

	void SetIndex(const int i);
	
	void ShowRange();
	void HideRange();

	virtual void Upgrade();

	static int GetPrice(TowerTypes type);
	static int GetDamage(TowerTypes type);
	static int GetFireSpeed(TowerTypes type);
	static int GetRadius(TowerTypes type);
	static char* GetImage(TowerTypes type);
	static char* GetFiringSprite(TowerTypes type);
};

