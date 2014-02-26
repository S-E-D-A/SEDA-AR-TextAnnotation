#ifndef __GAME_H
#define __GAME_H

#include <TooN/TooN.h>

//Abstract base class for graphics classes
class Game
{
public:
    Game( const TooN::SE3<> se3)
        : mse3CanvasFromWorld(se3) {};
    virtual void Init(void) =0;
    virtual void Draw(void) =0;
    //virtual void Draw(TooN::Vector<3> v3CameraPose) =0;
    virtual void Reset(void) =0;
    const TooN::SE3<> mse3CanvasFromWorld;

protected:
    bool mbInitialized;
};

#endif
