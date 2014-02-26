#ifndef __TEXTGAME_H
#define __TEXTGAME_H

#include <TooN/TooN.h>
using namespace TooN;
#include "OpenGL.h"
#include "Game.h"

class TextGame : public Game
{
public:
    TextGame(const SE3<> se3);
    virtual void Draw();
    virtual void Reset();
    virtual void Init();

};


#endif
