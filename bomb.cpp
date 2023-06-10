#include"bomb.h"
#include"def.h"

Bomb::Bomb(int x,int y,int master)
{
    this->x=x;
    this->y=y;
    this->master=master;
    lastTime=4000/FRAME;
    next=0;
}
Bomb::Bomb(int x,int y)
{
    this->x=x;
    this->y=y;
    lastTime=4000/FRAME;
    next=0;
}
