#ifndef ENEMY_H
#define ENEMY_H

class Enemy{
public:
    Enemy(int x,int y,int isBoss,int moveTime);
    int x;
    int y;
    int isBoss;                                         //boss不死
    int moveTime;
    int isAlive;
};

#endif // ENEMY_H
