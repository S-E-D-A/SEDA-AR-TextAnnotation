/*
 * Autor : Arnaud GROSJEAN (VIDE SARL)
 * This implementation of VideoSource allows to use OpenCV as a source for the video input
 * I did so because libCVD failed getting my V4L2 device
 *
 * INSTALLATION :
 * - Copy the VideoSource_Linux_OpenCV.cc file in your PTAM directory
 * - In the Makefile:
 *- set the linkflags to
 LINKFLAGS = -L MY_CUSTOM_LINK_PATH -lblas -llapack -lGVars3 -lcvd -lcv -lcxcore -lhighgui
 *- set the videosource to 
 VIDEOSOURCE = VideoSource_Linux_OpenCV.o
 * - Compile the project
 * - Enjoy !
 * 
 * Notice this code define two constants for the image width and height (PTAM_WIDTH and PTAM_HEIGHT)
 */

#include "VideoSource.h"
#include <cvd/Linux/v4lbuffer.h>
#include <cvd/colourspace_convert.h>
#include <cvd/colourspaces.h>
#include <gvars3/instances.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace CVD;
using namespace std;
using namespace GVars3;
using namespace cv;

#define FULL_WIDTH 960
#define FULL_HEIGHT 720
#define PTAM_WIDTH 640
#define PTAM_HEIGHT 480

void VideoSource::ResetVideo()
{
  string sVideoFile = GV3::get<string>("Videofile");
  if (sVideoFile == "")
    mptr = new VideoCapture(0); //use default webcam
  else
    mptr = new VideoCapture(sVideoFile);

  VideoCapture* cap = (VideoCapture*)mptr;
  if(!cap->isOpened())
    {
      cerr << "Unable to get the camera" << endl;
      exit(-1);
    }
  cap->set(CV_CAP_PROP_FRAME_WIDTH, FULL_WIDTH);
  cap->set(CV_CAP_PROP_FRAME_HEIGHT, FULL_HEIGHT);
  cout << "width is: " << cap->get(3) << " height is: " << cap->get(4) << endl;

  mirSize = ImageRef(PTAM_WIDTH, PTAM_HEIGHT);
  mirFullSize = ImageRef(FULL_WIDTH, FULL_HEIGHT);
}

VideoSource::VideoSource()
{
  cout << "  VideoSource_Linux: Opening video source..." << endl;
  ResetVideo();
  cout << "  ... got video source." << endl;

  //mirSize = ImageRef(PTAM_WIDTH, PTAM_HEIGHT);
};

ImageRef VideoSource::Size()
{ 
  return mirSize;
};

ImageRef VideoSource::FullSize()
{ 
  return mirFullSize;
};

void conversionNB(Mat frame, Image<byte> &imBW)
{
  Mat clone = frame.clone();
  Mat_<Vec3b>& frame_p = (Mat_<Vec3b>&)clone;
  for (int i = 0; i < PTAM_HEIGHT; i++){
    for (int j = 0; j < PTAM_WIDTH; j++){
      imBW[i][j] = (frame_p(i,j)[0] + frame_p(i,j)[1] + frame_p(i,j)[2]) / 3;
    }
  }
}

void conversionRGB(Mat frame, Image<Rgb<byte> > &imRGB)
{
  Mat clone = frame.clone();
  Mat_<Vec3b>& frame_p = (Mat_<Vec3b>&)clone;
  for (int i = 0; i < clone.cols; i++){
    for (int j = 0; j < clone.rows; j++){
      imRGB[i][j].red = frame_p(i,j)[2];
      imRGB[i][j].green = frame_p(i,j)[1];
      imRGB[i][j].blue = frame_p(i,j)[0];
    }
  }
}

void VideoSource::GetAndFillFrameBWandRGB(Image<byte> &imBW, Image<Rgb<byte> > &imRGB, Image<Rgb<byte> > &imFull)
{
  Mat frame;
  VideoCapture* cap = (VideoCapture*)mptr;

  bool gotFrame = cap->grab();
  cap->retrieve(frame);
    
  if (!gotFrame)
    {
      ResetVideo();
      cap = (VideoCapture*)mptr;
      cap->grab();
      cap->retrieve(frame);
    }
    
  conversionRGB(frame, imFull);

  resize(frame, frame, cv::Size(PTAM_WIDTH, PTAM_HEIGHT));
  conversionNB(frame, imBW);
  conversionRGB(frame, imRGB);
}

