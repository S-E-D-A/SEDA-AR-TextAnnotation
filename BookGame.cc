// Copyright 2008 Isis Innovation Limited
#include "BookGame.h"
#include <cvd/convolution.h>

using namespace CVD;
using namespace std;

BookGame::BookGame()
{
  mbInitialised = false;
  mdScale = 0.1;
  mbHasSummary = false;
  mnWordsPerLine = 4;
}

void BookGame::Reset()
{
  mnFrameCounter = 0;
  mnSummaryCounter = 0;
	mnChapter = 0;
}

void BookGame::UpdateScale(double dScale)
{
  //mdScale = dScale;
}

void BookGame::UpdateSummary(ARSummary* pChapSummary)
{
  mbHasSummary = true;
  mpChapSummary = pChapSummary;
  // delete pChapSummary;
	mnChapter = pChapSummary->nChapter;
	if (mnChapter == 0)
		mnWordsPerLine = 3;
	else
		mnWordsPerLine = 4;

  //Delete previous label+summary
  if (mnSummaryCounter != 0)
    {
      mnPrevLabelDisplayList = mnLabelDisplayList;
      glDeleteLists(mnPrevLabelDisplayList,5);

      mnPrevSummaryDisplayList = mnSummaryDisplayList;
      glDeleteLists(mnPrevSummaryDisplayList,1);
    }

  //Generate DisplayList for current labels
  mnLabelDisplayList = glGenLists(5);
  for (int i=0; i<5; i++)
    {
      glNewList(mnLabelDisplayList+i, GL_COMPILE);
      DrawString(mpChapSummary->vTopWords[i]);
      glEndList();
    }

  //Generate DisplayList for current summary
  int nSummaryLength = mpChapSummary->nNumSumWords;
  string sParagraph = "";
  for (int i=0; i < ceil((mpChapSummary->nNumSumWords)/mnWordsPerLine); i++) //row counter
    {
      for (int j=0; j < mnWordsPerLine; j++)
	{
	  if (i*mnWordsPerLine+j >= nSummaryLength)
	    break;
	  sParagraph+=mpChapSummary->vSummary[i*mnWordsPerLine+j];
	  sParagraph+=" ";
	}
      sParagraph+="\n";
    }
  mnSummaryDisplayList = glGenLists(1);
  glNewList(mnSummaryDisplayList, GL_COMPILE);
  DrawString(sParagraph);
  glEndList();

  mnSummaryCounter++;

}

void BookGame::DrawStuff(Vector<3> v3CameraPos)
{
  if(!mbInitialised)
    Init();

  mnFrameCounter++;
  
  if (mbHasSummary)
  {
    //Lighting Effects
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
   
	if (mnChapter == 0)
	{
		glColor3f(0.15f, 0.15f, 0.15f); //Grey
			
		//Render histogram bars
		for (int i=0; i<5; i++)
		{
			double dHeight = mpChapSummary->vTopWordFreqs[i];
			glLoadIdentity();
			glScaled(mdScale, mdScale, mdScale);
			glTranslatef(8.0f + 1.5f*i, 0.0f, 5*(dHeight/2));
			glScaled(1.0f, 1.0f, 5*dHeight);
			glCallList(mvHistDisplayList[i]);
		}

		//Render histogram labels
		for (int i=0; i<5; i++)
		{
			glLoadIdentity();
			glScaled(mdScale, mdScale, mdScale);
			glTranslatef(8.0f + 1.5f*i, 0.0f, 0.0f);
			glTranslatef(-0.25f, -0.75f, 0.0f);
			glRotated(-90, 0, 0, 1);
			glScalef(0.5f, 0.5f, 0.5f);
			glCallList(mnLabelDisplayList+i);
		}	

		//Render summary
		glLoadIdentity();
		glScaled(mdScale, mdScale, 0.0f);
		glScaled(0.75f, 0.75f, 0.0f);
		glTranslatef(-25.0f, 0.0f, 0.0f);
		glCallList(mnSummaryDisplayList);
	}
	else
	{
		glColor3f(0.15f, 0.15f, 0.15f); //Grey
			
		//Render histogram bars
		for (int i=0; i<5; i++)
		{
			double dHeight = mpChapSummary->vTopWordFreqs[i];
			glLoadIdentity();
			glScaled(mdScale, mdScale, mdScale);
			glTranslatef(8.0f + 1.5f*i, 0.0f, 5*(dHeight/2));
			glScaled(1.0f, 1.0f, 5*dHeight);
			glCallList(mvHistDisplayList[i]);
		}

		//Render histogram labels
		for (int i=0; i<5; i++)
		{
			glLoadIdentity();
			glScaled(mdScale, mdScale, mdScale);
			glTranslatef(8.0f + 1.5f*i, 0.0f, 0.0f);
			glTranslatef(-0.25f, -0.75f, 0.0f);
			glRotated(-90, 0, 0, 1);
			glScalef(0.5f, 0.5f, 0.5f);
			glCallList(mnLabelDisplayList+i);
		}	

		//Render summary
		glLoadIdentity();
		glScaled(mdScale, mdScale, 0.0f);
		glScaled(0.5f, 0.5f, 0.0f);
		glTranslatef(-35, 10.0f, 0.0f);
		glCallList(mnSummaryDisplayList);
	}
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	}

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

void BookGame::DrawString(string sWord)
{
  glDrawText(sWord);
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

  glSetFont("sans");

};


