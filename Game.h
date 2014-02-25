#ifndef __GAME_H
#define __GAME_H

#include <TooN/TooN.h>
using namespace TooN;

//Abstract base class for graphics classes
class Game
{
public:
    virtual void Init(void) =0;
    //virtual void Draw(void) =0;
    virtual void Draw(Vector<3> v3CameraPose) =0;
    virtual void Reset(void) =0;

protected:
    bool mbInitialized;
};

#endif
