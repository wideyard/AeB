#ifndef ITEM_H
#define ITEM_H

class Item
{
public:
    Item(int id, int x, int y);
    Item(int id,int lastTime);
    int id;
    int lastTime;
    int master;
    int x,y;
};
#endif // ITEM_H
