#include "TextGame.h"

using namespace CVD;
using namespace std;

TextGame::TextGame(const TooN::SE3<> se3, string sText)
    : Game(se3), msText(sText)
{
    mbInitialized = false;
    mdScale = 0.1;
}

void TextGame::Draw()
{
    if (!mbInitialized)
        Init();

    glMatrixMode(GL_MODELVIEW);
    glColor3f(0.15f, 0.15f, 0.15f); //Grey
    glLoadIdentity();
    glScaled(mdScale, mdScale, 0.0f);
    glCallList(mnTextDisplayList);

}

void TextGame::Init()
{
    mbInitialized = true;
    glSetFont("sans");

    mnTextDisplayList = glGenLists(1);
    glNewList(mnTextDisplayList, GL_COMPILE);
    DrawString();
    glEndList();
}

void TextGame::DrawString()
{
    glDrawText(msText);
}
