#include"enemy.h"

Enemy::Enemy(int x, int y, int isBoss, int moveTime)
{
    this->x=x;
    this->y=y;
    this->isBoss=isBoss;
    this->moveTime=moveTime;
    this->isAlive=1;
}
