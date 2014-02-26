#ifndef __TEXTGAME_H
#define __TEXTGAME_H

#include <string>
#include <TooN/TooN.h>
#include "OpenGL.h"
#include "Game.h"

class TextGame : public Game
{
public:
    TextGame(const TooN::SE3<> se3, std::string sText);
    virtual void Draw();

protected:
    void Init();
    void DrawString();

    GLuint mnTextDisplayList;
    double mdScale;
    std::string msText;

};


#endif
