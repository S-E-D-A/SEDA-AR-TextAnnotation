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

    //Lighting Effects
    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    GLfloat af[4];
    af[0]=0.5;
    af[1]=0.5;
    af[2]=0.5;
    af[3]=1.0;
    glLightfv(GL_LIGHT0, GL_AMBIENT, af);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, af);
    af[0]=1.0;
    af[1]=0.0;
    af[2]=1.0;
    af[3]=0.0;
    glLightfv(GL_LIGHT0, GL_POSITION, af);
    af[0]=1.0;
    af[1]=1.0;
    af[2]=1.0;
    af[3]=1.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, af);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);


    glMatrixMode(GL_MODELVIEW);
    glColor3f(0.85f, 0.85f, 0.85f); //Grey
    glLoadIdentity();
    glMultMatrix(mse3CanvasFromWorld.inverse());
    glScaled(mdScale, mdScale, 0.0f);
    glCallList(mnTextDisplayList);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

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
