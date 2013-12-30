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
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
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
  
  //Translate, Rotate, Scale each Eyeball
  for(int i=0; i<4; i++)
    {
      if(mnFrameCounter < 100)
	LookAt(i, 500.0 * (Vector<3>) makeVector( (i<2?-1:1)*(mnFrameCounter < 50 ? -1 : 1) * -0.4 , -0.1, 1) , 0.05 );
      else
	LookAt(i, v3CameraPos, 0.02 );
      
      glLoadIdentity();
      glMultMatrix(ase3WorldFromEye[i]);
      glScaled(mdBaseline, mdBaseline, mdBaseline);
      glCallList(mnEyeDisplayList);
    }
  glDisable(GL_CULL_FACE);

  if (mbHasSummary)
  {
    for (int i=0; i<5; i++)
      {
	glLoadIdentity();
	double width = mdBaseline;
	double height = width*4*mpChapSummary->vTopWordFreqs[i];

	glTranslatef(width*4 + i*1.5*width, 0.0f, width*(height/2));
	glScaled(width, width, width*height);
	glCallList(mnHistDisplayList[i]);
      }
  }

  //Draw Shadows
  glDisable(GL_LIGHTING);
  glLoadIdentity();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, mnShadowTex);
  glEnable(GL_BLEND);
  glColor4f(0,0,0,0.5);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex2d(-mdShadowHalfSize, -mdShadowHalfSize);
  glTexCoord2f(0,1);
  glVertex2d(-mdShadowHalfSize,  mdShadowHalfSize);
  glTexCoord2f(1,1);
  glVertex2d( mdShadowHalfSize,  mdShadowHalfSize);
  glTexCoord2f(1,0);
  glVertex2d( mdShadowHalfSize, -mdShadowHalfSize);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);


  // cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 240, 80);
  // cairo_t *cr = cairo_create (surface);

  // PangoLayout *layout;
  // PangoFontDescription *font_description;

  // font_description = pango_font_description_new ();
  // pango_font_description_set_family (font_description, "serif");
  // pango_font_description_set_weight (font_description, PANGO_WEIGHT_BOLD);
  // pango_font_description_set_absolute_size (font_description, 32 * PANGO_SCALE);

  // layout = pango_cairo_create_layout (cr);
  // pango_layout_set_font_description (layout, font_description);
  // pango_layout_set_text (layout, "Hello, world", -1);

  // cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
  // cairo_move_to (cr, 10.0, 50.0);
  // pango_cairo_show_layout (cr, layout);

  // g_object_unref (layout);
  // pango_font_description_free (font_description);

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


void BookGame::DrawEye()
{
  int nSegments = 45;
  int nSlices = 45;
  
  double dSliceAngle = M_PI / (double)(nSlices);
  double dSegAngle = 2.0 * M_PI / (double)(nSegments);
  
  glColor3f(0.0,0.0,0.0);
  {  // North pole:
    double Z = sin(M_PI/2.0 - dSliceAngle);
    double R = cos(M_PI/2.0 - dSliceAngle);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,1);
    glVertex3f(0,0,1);
    for(int i=0; i<nSegments;i++)
      {
	glNormal3f(R * sin((double)i * dSegAngle), R * cos((double)i * dSegAngle),  Z);
	glVertex3f(R * sin((double)i * dSegAngle), R * cos((double)i * dSegAngle),  Z);
      }
    glNormal3f(0,R,Z);
    glVertex3f(0,R,Z);
    glEnd();
  }
  
  int nBlueSlice = 3;
  int nWhiteSlice = 6;
  for(int j = 1; j<nSlices;j++)
    {
      if(j == nBlueSlice)
	glColor3f(0,0,1);
      if(j == nWhiteSlice)
	glColor4d(0.92, 0.9, 0.85,1);
      
      glBegin(GL_QUAD_STRIP);
      double zTop = sin(M_PI/2.0 - dSliceAngle * (double)j);
      double zBot = sin(M_PI/2.0 - dSliceAngle * (double)(j+1));
      double rTop = cos(M_PI/2.0 - dSliceAngle * (double)j);
      double rBot = cos(M_PI/2.0 - dSliceAngle * (double)(j+1));
      for(int i=0; i<nSegments;i++)
	{
	  glNormal3f(rTop*sin((double)i*dSegAngle), rTop*cos((double)i*dSegAngle), zTop);
	  glVertex3f(rTop*sin((double)i*dSegAngle), rTop*cos((double)i*dSegAngle), zTop);
	  glNormal3f(rBot*sin((double)i*dSegAngle), rBot*cos((double)i*dSegAngle), zBot);
	  glVertex3f(rBot*sin((double)i*dSegAngle), rBot*cos((double)i*dSegAngle), zBot);
	};
      glNormal3f(0,rTop, zTop);
      glVertex3f(0,rTop, zTop);
      glNormal3f(0,rBot, zBot);
      glVertex3f(0,rBot, zBot);
      glEnd();
    };

  {
    // South pole:
    double Z = sin(M_PI/2.0 - dSliceAngle);
    double R = cos(M_PI/2.0 - dSliceAngle);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,-1);
    glVertex3f(0,0,-1);
    for(int i=0; i<nSegments;i++)
      {
	glNormal3f(R * sin((double)i * -dSegAngle), R * cos((double)i * -dSegAngle),  -Z);
	glVertex3f(R * sin((double)i * -dSegAngle), R * cos((double)i * -dSegAngle),  -Z);
      }
    glNormal3f(0,R,-Z);
    glVertex3f(0,R,-Z);
    glEnd();
  };

}

void BookGame::DrawCube(GLfloat size, GLenum type)
{
  glColor3f(0.15f, 0.15f, 0.15f); //Grey
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

void BookGame::Init()
{
  if(mbInitialised) return;
  mbInitialised = true;

  // Set up the display list for the eyeball.
  mnEyeDisplayList = glGenLists(1);
  glNewList(mnEyeDisplayList,GL_COMPILE);
  DrawEye();
  glEndList();

  // Set up the display list for the histogram
  for (int i=0; i<5; i++)
    {
      mnHistDisplayList.push_back(glGenLists(1));
      glNewList(mnHistDisplayList[i],GL_COMPILE);
      DrawCube(1.0f, GL_QUADS);
      glEndList();
    }

  MakeShadowTex();
};


void BookGame::LookAt(int nEye, Vector<3> v3, double dRotLimit)
{
  Vector<3> v3E = ase3WorldFromEye[nEye].inverse() * v3;
  if(v3E * v3E == 0.0)
    return;
  
  normalize(v3E);
  Matrix<3> m3Rot = Identity;
  m3Rot[2] = v3E;
  m3Rot[0] -= m3Rot[2]*(m3Rot[0]*m3Rot[2]); 
  normalize(m3Rot[0]);
  m3Rot[1] = m3Rot[2] ^ m3Rot[0];
  
  SO3<> so3Rotator = m3Rot;
  Vector<3> v3Log = so3Rotator.ln();
  v3Log[2] = 0.0;
  double dMagn = sqrt(v3Log * v3Log);
  if(dMagn > dRotLimit)
    {
      v3Log = v3Log * ( dRotLimit / dMagn);
    }
  ase3WorldFromEye[nEye].get_rotation() = ase3WorldFromEye[nEye].get_rotation() * SO3<>::exp(-v3Log);
};

void BookGame::MakeShadowTex()
{
  const int nTexSize = 256;
  Image<byte> imShadow(ImageRef(nTexSize, nTexSize));
  double dFrac = 1.0 - mdBaseline / mdShadowHalfSize;
  double dCenterPos = dFrac * nTexSize / 2 - 0.5;
  ImageRef irCenter; irCenter.x = irCenter.y = (int) dCenterPos;
  int nRadius = int ((nTexSize / 2 - irCenter.x) * 1.05);
  unsigned int nRadiusSquared = nRadius*nRadius;
  ImageRef ir;
  for(ir.y = 0; 2 * ir.y < nTexSize; ir.y++)
    for(ir.x = 0; 2 * ir.x < nTexSize; ir.x++)
      {
	byte val = 0;
	if((ir - irCenter).mag_squared() < nRadiusSquared)
	  val = 255;
	imShadow[ir] = val;
	imShadow[ImageRef(nTexSize - 1 - ir.x, ir.y)] = val;
	imShadow[ImageRef(nTexSize - 1 - ir.x, nTexSize - 1 - ir.y)] = val;
	imShadow[ImageRef(ir.x, nTexSize - 1 - ir.y)] = val;
      }
  
  convolveGaussian(imShadow, 4.0);
  glGenTextures(1, &mnShadowTex);
  glBindTexture(GL_TEXTURE_2D,mnShadowTex);
  glTexImage2D(GL_TEXTURE_2D, 0, 
	       GL_ALPHA, nTexSize, nTexSize, 0, 
	       GL_ALPHA, GL_UNSIGNED_BYTE, imShadow.data()); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
};





