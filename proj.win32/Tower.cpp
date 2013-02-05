#include "Tower.h"

using namespace cocos2d;

Tower::Tower(MoneyManager *moneyManager, TowerTypes type, CCPoint _position) : 
		UpgradeBase(moneyManager, UPGRADES_COUNT_TOWER){
	//CCLog("Type: %d\n", type);
	this->index = 0;

	this->damage = GetDamage(type);
	this->fireSpeed = GetFireSpeed(type);
	this->fireRadius = GetRadius(type);
	this->spritePtr = CCSprite::create(GetImage(type));
	this->SetUpgPriceForNextLevel(GetPrice(type) * 2);	

	this->position=_position;
	this->spritePtr->setPosition(_position);
	this->reloadTime = 0;	

	//CCLog("FireRadius: %d\n", this->fireRadius);
}


Tower::~Tower(void){
	//this->spritePtr->release();
}

void Tower::setX(float newX){
	this->position.x = newX;
}

void Tower::setY(float newY){
	this->position.y = newY;
}

CCSprite * Tower::getSprite() const{
	return this->spritePtr;
}

const bool Tower::operator < (const Tower &tower) const{
	/*int compareValue = this->position.x * 10000 + this->position.y;
	int compareValue2 = tower.getX() * 10000 + tower.getY();
	return compareValue < compareValue2;*/
	return (this < &tower);
}

void Tower::turnTo(const CCPoint point) const{
	//CCLog ("TurnTo called");
	double dx = this->position.x - point.x;
	double dy = this->position.y - point.y;
	int circleHalf;
	if (dy < 0){
		circleHalf = LOWER_HALF;
	}else{
		circleHalf = UPPER_HALF;
	}
	
	double tanOfAngle = (dx/dy);
	double angle = atan(tanOfAngle)*57.2957795135;
	if (circleHalf == UPPER_HALF){
		angle+=180;
	}
	if (angle >= 0 && angle <= 360){
		this->spritePtr->setRotation(angle);
	}
}

const bool Tower::operator> (const Tower &tower) const{
	return !(*this < tower);
}

const bool Tower::isTargetInRange(CCPoint target) const{
	//std::cout<<this->position.x<<"\t"<<this->position.y<<std::endl;
	//std::cout<<target.x<<"\t"<<target.y<<std::endl;
	double range = ccpDistance(spritePtr->getPosition(), target);
	//CCLog("%f\t%d\n", range, this->fireRadius);
	if (range <= this->fireRadius) {
		return true;
	}else{
		return false;
	}
}

void Tower::processEnemies(Wave *wave){
	for (int i = 0; i < wave->GetEnemyCount(); i++){
		if (this->isTargetInRange(wave->GetEnemyPosition(i))){
			this->turnTo(wave->GetEnemyPosition(i));
			this->fire(wave, i);
			break;
		}
	}
}

void Tower::fire(Wave *wave, int index){
	if (this->isAbleToFire()){
#ifdef DEBUG_LOGS
		CCLog("Making damage");
#endif
		wave->MakeDamage(index, this->damage);
		this->reloadTime = GAME_SPEED / this->fireSpeed;
	}else{
#ifdef DEBUG_LOGS
		CCLog("Reloading");
#endif
		this->reloadTime--;
	}
}

const bool Tower::isAbleToFire() const{
	return (this->reloadTime <= 0);
}

void Tower::Upgrade()
{
#ifdef DEBUG_LOGS
	CCLog("Perform tower upgrade");
#endif
	UpgradeBase::Upgrade();	

	fireSpeed *= UPGRADE_COEF_TOWER;
	fireRadius *= UPGRADE_COEF_TOWER;
	damage *= UPGRADE_COEF_TOWER;
	reloadTime *= UPGRADE_COEF_TOWER;
}

void Tower::SetIndex(const int i)
{
	this->index = i;
}

int Tower::GetIndex()
{
	return this->index;
}

void Tower::ShowRange()
{	
	this->spriteRangePtr = CCSprite::create(FILE_NAME_IMAGE_TOWER_RANGE);
	this->spriteRangePtr->setPosition(this->spritePtr->getPosition());
	this->spriteRangePtr->setScale((float)this->fireRadius / (this->spriteRangePtr->getContentSize().width / 2.0));
	this->spriteRangePtr->setOpacity(100);
	this->spritePtr->getParent()->addChild(this->spriteRangePtr, 5);	
}

void Tower::HideRange()
{	
	this->spriteRangePtr->removeFromParent(); //this->spritePtr->getParent()->removeChild(this->spriteRangePtr, true);
	this->spriteRangePtr = NULL;
}

int Tower::GetPrice(TowerTypes type)
{
	return (GetDamage(type) + GetFireSpeed(type) + GetRadius(type)) / 2;
}

int Tower::GetDamage(TowerTypes type)
{
	int damage = 0;
	switch (type){
		case MACHINE_GUN:
			damage = 10;			
			break;
		case HEAVY_GUN:
			damage = 20;			
			break;
	}
	return damage;
}

int Tower::GetFireSpeed(TowerTypes type)
{
	int speed = 0;
	switch (type){
		case MACHINE_GUN:
			speed = 5;			
			break;
		case HEAVY_GUN:
			speed = 7;			
			break;
	}
	return speed;
}

int Tower::GetRadius(TowerTypes type)
{
	int radius = 0;
	switch (type){
		case MACHINE_GUN:
			radius = 100;			
			break;
		case HEAVY_GUN:
			radius = 150;			
			break;
	}
	return radius;
}

char* Tower::GetImage(TowerTypes type)
{
	char *image = NULL;
	switch (type){
		case MACHINE_GUN:			
			image = FILE_NAME_IMAGE_TOWER_MACHINE_GUN;
			break;
		case HEAVY_GUN:
			image = FILE_NAME_IMAGE_TOWER_HEAVY_GUN;
			break;
	}
	return image;
}
