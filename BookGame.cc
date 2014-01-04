// Copyright 2008 Isis Innovation Limited
#include "BookGame.h"
#include <cvd/convolution.h>

using namespace CVD;

BookGame::BookGame()
{
  mbInitialised = false;
  mdBaseline = 0.1;
  mdShadowHalfSize = 2.5 * mdBaseline;
  mbHasSummary = false;
}

void BookGame::UpdateBaseline(double dBaseline)
{
  mdBaseline = dBaseline;
  mdShadowHalfSize = 2.5 * mdBaseline;
}

void BookGame::UpdateSummary(ARSummary* pChapSummary)
{
  mbHasSummary = true;
  mpChapSummary = pChapSummary;
  // delete pChapSummary;
}

void BookGame::DrawStuff(Vector<3> v3CameraPos)
{
  if(!mbInitialised)
    Init();

  mnFrameCounter++;
  //glDisable(GL_BLEND);
  //glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  GLfloat af[4]; 
  af[0]=0.5; af[1]=0.5; af[2]=0.5; af[3]=1.0;
  glLightfv(GL_LIGHT0, GL_AMBIENT, af);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, af);
  af[0]=1.0; af[1]=0.0; af[2]=1.0; af[3]=0.0;
  glLightfv(GL_LIGHT0, GL_POSITION, af);
  af[0]=1.0; af[1]=1.0; af[2]=1.0; af[3]=1.0;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, af);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
  
  glMatrixMode(GL_MODELVIEW);
  
  // //Translate, Rotate, Scale each Eyeball
  // for(int i=0; i<4; i++)
  //   {
  //     if(mnFrameCounter < 100)
  // 	LookAt(i, 500.0 * (Vector<3>) makeVector( (i<2?-1:1)*(mnFrameCounter < 50 ? -1 : 1) * -0.4 , -0.1, 1) , 0.05 );
  //     else
  // 	LookAt(i, v3CameraPos, 0.02 );
      
  //     glLoadIdentity();
  //     glMultMatrix(ase3WorldFromEye[i]);
  //     glScaled(mdBaseline, mdBaseline, mdBaseline);
  //     glCallList(mnEyeDisplayList);
  //   }
  // glDisable(GL_CULL_FACE);

  if (mbHasSummary)
  {

    //this is to ensure proper scaling for different baseline sizes
    double width = mdBaseline;

    //Render histogram bars
    for (int i=0; i<5; i++)
      {
	double height = width*4*mpChapSummary->vTopWordFreqs[i];
	glLoadIdentity();
	glColor3f(0.15f, 0.15f, 0.15f); //Grey
	glTranslatef(width*4 + i*1.5*width, 0.0f, width*(height/2));
	glScaled(width, width, width*height);
	glCallList(mvHistDisplayList[i]);
      }

    glDisable(GL_LIGHTING);

    //Render histogram labels
    for (int i=0; i<5; i++)
      {
	glLoadIdentity();
	glTranslatef(width*(4) + width*(1.5)*i, width*(-2), 0.0f);
    	glScaled(width/2, 2*width, 0.0f);
	glColor3f(0.15f, 0.15f, 0.15f); //Grey
	glTranslatef(-0.5f, 0.5f, 0.0f);
    	glScaled(1.0f, 0.25f, 0.0f);
	glRotated(-90,0,0,1);
	glDrawText("Gandolf");
      }

    //Render summary
    glLoadIdentity();
    glColor3f(0.15f, 0.15f, 0.15f); //Grey    
    //glScaled(width, width/4, 0.0f);
    glScaled(width, width, 0.0f);
    glTranslatef(width*(-150), 0.0f, 0.0f);
    glDrawText("TEST TEST");
    glTranslatef(0.0f, width*(-20), 0.0f);    
    glDrawText("Hj Tack");
    
  }

  glDisable(GL_DEPTH_TEST);

};


void BookGame::Reset()
{
  for(int i=0; i<4; i++)
    ase3WorldFromEye[i] = SE3<>();

  ase3WorldFromEye[0].get_translation()[0] = -mdBaseline;
  ase3WorldFromEye[1].get_translation()[0] = mdBaseline;
  ase3WorldFromEye[2].get_translation()[0] = -mdBaseline;
  ase3WorldFromEye[3].get_translation()[0] = mdBaseline;

  ase3WorldFromEye[0].get_translation()[1] = -mdBaseline;
  ase3WorldFromEye[1].get_translation()[1] = -mdBaseline;
  ase3WorldFromEye[2].get_translation()[1] = mdBaseline;
  ase3WorldFromEye[3].get_translation()[1] = mdBaseline;

  ase3WorldFromEye[0].get_translation()[2] = mdBaseline;
  ase3WorldFromEye[1].get_translation()[2] = mdBaseline;
  ase3WorldFromEye[2].get_translation()[2] = mdBaseline;
  ase3WorldFromEye[3].get_translation()[2] = mdBaseline;
  mnFrameCounter = 0;
};

void BookGame::DrawCube(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
  static GLint faces[6][4] =
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void BookGame::DrawSquare(GLfloat size, GLenum type)
{
  glBegin(type);
  glNormal3f(0.0f, 0.0f, size);
  glVertex3f( size, size, 0);
  glVertex3f(-size, size, 0);
  glVertex3f(-size,-size, 0);
  glVertex3f( size,-size, 0);
  glEnd();
}

void BookGame::Init()
{
  if(mbInitialised) return;
  mbInitialised = true;

  // Set up the display list for the histogram
  for (int i=0; i<5; i++)
    {
      mvHistDisplayList.push_back(glGenLists(1));
      glNewList(mvHistDisplayList[i],GL_COMPILE);
      DrawCube(1.0f, GL_QUADS);
      glEndList();
    }

  // //Set up the display list for the histogram
  // for (int i=0; i<5; i++)
  //   {
  //     mvTextDisplayList.push_back(glGenLists(1));
  //     glNewList(mvTextDisplayList[i],GL_COMPILE);
  //     DrawSquare(1.0f, GL_QUADS);
  //     glEndList();
  //   }
  
  // mnSummaryDisplayList = glGenLists(1);
  // glNewList(mnSummaryDisplayList,GL_COMPILE);
  // DrawSquare(1.0f, GL_QUADS);
  // glEndList();

  glSetFont("sans");

};


