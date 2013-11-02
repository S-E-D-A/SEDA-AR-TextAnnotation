// Copyright 2008 Isis Innovation Limited
#include "VideoSource.h"
#include <cvd/videofilebuffer.h>
#include <cvd/colourspace_convert.h>
#include <cvd/colourspaces.h>
#include <gvars3/instances.h>

using namespace CVD;
using namespace std;
using namespace GVars3;

VideoSource::VideoSource() 
{
cout << "  VideoSource_Linux: Opening video source..." << endl;
// string QuickCamFile = GV3::get<string>("VideoSource.V4LDevice", "/dev/video0");
// ImageRef irSize = GV3::get<ImageRef>("VideoSource.Resolution", ImageRef(640,480));
// int nFrameRate = GV3::get<int>("VideoSource.Framerate", 30);
// V4LBuffer<yuv422>* pvb = new V4LBuffer<yuv422>(QuickCamFile, irSize, -1, false, nFrameRate);

string VideoFile = "file:///home/vagrant/SEDA-CV-DetectionAndAnnotation/Annotation/TestLibCVD/iphone_001.m4v";
VideoFileBuffer<Rgb<byte> > * pvb = new VideoFileBuffer<Rgb<byte> >(VideoFile);
mirSize = pvb->size();
mptr = pvb;
cout << "  ... got video source." << endl;
};

ImageRef VideoSource::Size()
{ 
return mirSize;
};

void VideoSource::GetAndFillFrameBWandRGB(Image<byte> &imBW, Image<Rgb<byte> > &imRGB)
{
VideoFileBuffer<Rgb<byte> >* pvb = (VideoFileBuffer<Rgb<byte> >*) mptr;
VideoFrame<Rgb<byte> > *pVidFrame = pvb->get_frame();
convert_image(*pVidFrame, imBW);
convert_image(*pVidFrame, imRGB);
pvb->put_frame(pVidFrame);
}
