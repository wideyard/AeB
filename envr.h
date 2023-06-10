#ifndef ENVR_H
#define ENVR_H

#include <QString>
#include"def.h"

extern int build[BUILDSIZE][2];

class Plant{
public:
    Plant(int x,int y);
    int x;
    int y;
};

#endif // ENVR_H
