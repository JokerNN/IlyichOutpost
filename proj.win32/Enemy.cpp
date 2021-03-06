#include "Enemy.h"

#include "PassingMap.h"
#include "Wave.h"

using namespace cocos2d;

Enemy::Enemy(MoneyManager *moneyManager, const EnemyType eType, CCScene *scene, Waypoint *way) :
	UpgradeBase(moneyManager, UPGRADES_COUNT_ENEMY)
{
	this->moneyManager = moneyManager;
	this->waypoint = way;
	this->scene = scene;
	this->currentPoint = 0;
	this->BaseReachedCallback = NULL;

	char *image = NULL;
	switch(eType)
	{
		case ENEMY_SOLDIER:
			this->healthCurrent = this->healthTotal = 100;
			this->speed = 10;
			image = FILE_NAME_IMAGE_ENEMY_SOLDIER;
			break;
		case ENEMY_HEAVY_SOLDIER:
			this->healthCurrent = this->healthTotal = 150;
			this->speed = 10;
			image = FILE_NAME_IMAGE_ENEMY_HEAVY_SOLDIER;
			break;
		case ENEMY_HORSEMAN:
			this->healthCurrent = this->healthTotal = 250;
			this->speed = 25;
			image = FILE_NAME_IMAGE_ENEMY_HORSEMAN;
			break;
		case ENEMY_HEAVY_HORSEMAN:
			this->healthCurrent = this->healthTotal = 400;
			this->speed = 20;
			image = FILE_NAME_IMAGE_ENEMY_HEAVY_HORSEMAN;
			break;
		case ENEMY_MACHINEGUN_CART:
			this->healthCurrent = this->healthTotal = 700;
			this->speed = 20;
			image = FILE_NAME_IMAGE_ENEMY_MACHINEGUN_CART;
			break;
		case ENEMY_HEAVY_MACHINEGUN_CART:
			this->healthCurrent = this->healthTotal = 950;
			this->speed = 15;
			image = FILE_NAME_IMAGE_ENEMY_HEAVY_MACHINEGUN_CART;
			break;
		case ENEMY_ARMORED_CAR:
			this->healthCurrent = this->healthTotal = 1300;
			this->speed = 15;
			image = FILE_NAME_IMAGE_ENEMY_ARMORED_CAR;
			break;
		case ENEMY_TANK:
			this->healthCurrent = this->healthTotal = 1900;
			this->speed = 10;
			image = FILE_NAME_IMAGE_ENEMY_TANK;
			break;
		case ENEMY_HEAVY_TANK:
			this->healthCurrent = this->healthTotal = 2500;
			this->speed = 7;
			image = FILE_NAME_IMAGE_ENEMY_HEAVY_TANK;
			break;
		default:
			CCLog("Wrong enemy type!");
			this->healthCurrent = this->healthTotal = 50;
			this->speed = 10;
			image = FILE_NAME_IMAGE_ENEMY_SOLDIER;
			break;
	}

	this->SetUpgPriceForNextLevel(this->healthCurrent);

	CCSprite *spriteEnemyLocal = CCSprite::create(image);
	float scaleX = GameSettings::MAP_CELL_SIZE / spriteEnemyLocal->getContentSize().width;
	float scaleY = GameSettings::MAP_CELL_SIZE / spriteEnemyLocal->getContentSize().height;
	spriteEnemyLocal->setScaleX(scaleX);
	spriteEnemyLocal->setScaleY(scaleY);
	spriteEnemyLocal->setPosition(ccp(GameSettings::MAP_CELL_SIZE / 2, GameSettings::MAP_CELL_SIZE / 2));

	this->sprite = CCSprite::create();
	this->sprite->addChild(spriteEnemyLocal, 1);
	this->sprite->setContentSize(CCSize(GameSettings::MAP_CELL_SIZE, GameSettings::MAP_CELL_SIZE));
	this->sprite->setPosition(ccp(-100, -100));

	// add health line
	this->spriteHealth = CCSprite::create(FILE_NAME_IMAGE_HEALTH_LINE);

	int posX = this->sprite->getContentSize().width / 2;
	int posY = this->sprite->getContentSize().height;
	this->spriteHealth->setPositionX(posX);
	this->spriteHealth->setPositionY(posY);	

	this->sprite->addChild(this->spriteHealth, 2);

	this->scene->addChild(this->sprite);
}

Enemy::~Enemy(void)
{
	Destroy();
}

void Enemy::SetSpeed(const int speed)
{
	this->speed = speed;
}

void Enemy::Destroy()
{
	this->sprite->cleanup();
	this->sprite->removeAllChildrenWithCleanup(true);
	this->sprite->removeFromParent();//this->scene->removeChild(this->sprite, true);

	CCLog("Enemy is destroyed");
}

void Enemy::CheckPointReached()
{
  Cell *curCell = waypoint->GetPoint(currentPoint);
  Cell *nextCell = waypoint->GetPoint(currentPoint + 1);

  if (curCell && nextCell)
  {
	  this->sprite->setPosition(ccp(curCell->x, curCell->y));

	  //calculate duration
	  float actualDuration = ccpDistance(ccp(curCell->x, curCell->y), ccp(nextCell->x, nextCell->y)) / this->speed;

      // Create the actions
      CCFiniteTimeAction* actionMove = CCMoveTo::create( actualDuration, ccp(nextCell->x, nextCell->y) );

      CCFiniteTimeAction* actionMoveDone = CCCallFunc::create( this, callfunc_selector(Enemy::CheckPointReached));
      this->sprite->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );

	  this->currentPoint++;

	  // For testing
	  //MakeDamage(60);
  }
  else
  {
	  CCLog("Enemy reached base");
	  if (this->BaseReachedCallback)
		  BaseReachedCallback();
  }
}

void Enemy::RefreshHealth()
{
	this->spriteHealth->setScaleX((float)this->healthCurrent / this->healthTotal);
}

// true - if killed
bool Enemy::MakeDamage(const int health)
{
	this->healthCurrent -= health;
	if (healthCurrent <= 0)
	{
		this->healthCurrent = 0;		

		this->sprite->cleanup();

		//CCParticleExplosion *particle = CCParticleExplosion::createWithTotalParticles(100);
		//particle->setTexture(sprite->getTexture());

		CCFiniteTimeAction *actionWait = CCDelayTime::create(TIMEOUT_ANIMATION);
		CCFiniteTimeAction* actionKill = CCTwirl::create(ccp(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2), 3, 0.3f, ccg(1,1), TIMEOUT_ANIMATION);


		CCFiniteTimeAction* actionDestroy = CCCallFunc::create( this, callfunc_selector(Enemy::Destroy));
        this->sprite->runAction( CCSequence::create(actionWait, actionKill, actionDestroy, NULL) );

		moneyManager->AddMoney((int)sqrt((float)this->healthTotal));

		return true;
	}
	else
	{		
		CCFiniteTimeAction *actionWait = CCDelayTime::create(TIMEOUT_ANIMATION);
        CCFiniteTimeAction* actionRefreshHealth = CCCallFunc::create( this, callfunc_selector(Enemy::RefreshHealth));
        this->sprite->runAction( CCSequence::create(actionWait, actionRefreshHealth, NULL) );
		return false;
	}
}

void Enemy::Start()
{ 
	CheckPointReached();
}

CCPoint Enemy::GetPosition()
{
		return this->sprite->getPosition();
}

void Enemy::Upgrade()
{
#ifdef DEBUG_LOGS
	CCLog("Perform enemy upgrade");
#endif	
	UpgradeBase::Upgrade();
	healthTotal *= UPGRADE_COEF_ENEMY;
	speed *= UPGRADE_COEF_ENEMY;
}

void Enemy::update(float dt){

}