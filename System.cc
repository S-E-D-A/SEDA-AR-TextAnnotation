// Copyright 2008 Isis Innovation Limited
#include "System.h"
#include "OpenGL.h"
#include <gvars3/instances.h>
#include <stdlib.h>
#include "ATANCamera.h"
#include "MapMaker.h"
#include "Tracker.h"
#include "ARDriver.h"
#include "MLDriver.h"
#include "MapViewer.h"

using namespace CVD;
using namespace std;
using namespace GVars3;


System::System()
  : mGLWindow(mVideoSource.Size(), "PTAM")
{
  GUI.RegisterCommand("exit", GUICommandCallBack, this);
  GUI.RegisterCommand("quit", GUICommandCallBack, this);
  
  mimFrameBW.resize(mVideoSource.Size());
  mimFrameRGB.resize(mVideoSource.Size());
  mimFrameFull.resize(mVideoSource.FullSize());
  // First, check if the camera is calibrated.
  // If not, we need to run the calibration widget.
  Vector<NUMTRACKERCAMPARAMETERS> vTest;
  
  vTest = GV3::get<Vector<NUMTRACKERCAMPARAMETERS> >("Camera.Parameters", ATANCamera::mvDefaultParams, HIDDEN);
  mpCamera = new ATANCamera("Camera");
  Vector<2> v2;
  if(v2==v2) ;
  if(vTest == ATANCamera::mvDefaultParams)
    {
      cout << endl;
      cout << "! Camera.Parameters is not set, need to run the CameraCalibrator tool" << endl;
      cout << "  and/or put the Camera.Parameters= line into the appropriate .cfg file." << endl;
      exit(1);
    }
  
  mpMap = new Map;
  mpMapMaker = new MapMaker(*mpMap, *mpCamera);
  mpTracker = new Tracker(mVideoSource.Size(), *mpCamera, *mpMap, *mpMapMaker);
  mpARDriver = new ARDriver(*mpCamera, mVideoSource.Size(), mGLWindow);
  mpMLDriver = new MLDriver();
  mpMapViewer = new MapViewer(*mpMap, mGLWindow);
  
  GUI.ParseLine("GLWindow.AddMenu Menu Menu");
  GUI.ParseLine("Menu.ShowMenu Root");
  GUI.ParseLine("Menu.AddMenuButton Root Reset Reset Root");
  GUI.ParseLine("Menu.AddMenuButton Root Spacebar PokeTracker Root");
  GUI.ParseLine("DrawAR=0");
  GUI.ParseLine("DrawMap=0");
  GUI.ParseLine("Menu.AddMenuToggle Root \"View Map\" DrawMap Root");
  GUI.ParseLine("Menu.AddMenuToggle Root \"Draw AR\" DrawAR Root");
  GUI.ParseLine("Chapter=0");
  GUI.ParseLine("Menu.AddMenuSlider Root \"Chapter\" Chapter 0 3 Root"); 
  
  mbDone = false;
  mbNewSummary = false;
  mnFrame = 0;
  mnChapter = -1;
};

void System::Run()
{
	while(!mbDone)
    {
      
		// We use two versions of each video frame:
		// One black and white (for processing by the tracker etc)
		// and one RGB, for drawing.

		// Grab new video frame...
		mVideoSource.GetAndFillFrameBWandRGB(mimFrameBW, mimFrameRGB, mimFrameFull);
		mnFrame++;
		static bool bFirstFrame = true;
		if(bFirstFrame)
		{
		mpARDriver->Init();
		bFirstFrame = false;
	}
      
	mGLWindow.SetupViewport();
	mGLWindow.SetupVideoOrtho();
	mGLWindow.SetupVideoRasterPosAndZoom();
		  
	if(!mpMap->IsGood())
		mpARDriver->Reset();
		  
	static gvar3<int> gvnDrawMap("DrawMap", 0, HIDDEN|SILENT);
	static gvar3<int> gvnDrawAR("DrawAR", 0, HIDDEN|SILENT);

	bool bDrawMap = mpMap->IsGood() && *gvnDrawMap;
	bool bDrawAR = mpMap->IsGood() && *gvnDrawAR;
		  
	mpTracker->TrackFrame(mimFrameBW, !bDrawAR && !bDrawMap);

	// bNewSummary will be the return value of some CV related function
	// such as template matching
	ARSummary* pChapSummary;

	// This will cycle through the chapters automatically for demo purposes
	//if (mnFrame % 100 == 0)
	//{
	//	cout << "Get Summary" << endl;
	//	pChapSummary = mpMLDriver->GetSummary(mnChapter++);
	//	if (mnChapter == 4)
	//		mnChapter = 1;
	//	mbNewSummary = true;
	//}
	
	// Uses the gvar Chapter to set chapter number
	static gvar3<int> gvnChapter("Chapter", 0, HIDDEN|SILENT);
	if (*gvnChapter != mnChapter)
	{
		cout << "New Chapter" << endl;
		pChapSummary = mpMLDriver->GetSummary(*gvnChapter);
		mbNewSummary = true;
		mnChapter = *gvnChapter;
	}

	if(bDrawMap)
		mpMapViewer->DrawMap(mpTracker->GetCurrentPose());
	else if(bDrawAR)
	{
		if (mbNewSummary)
		{
			mpARDriver->mpGame->UpdateSummary(pChapSummary);
			mbNewSummary = false;
		}
		mpARDriver->Render(mimFrameRGB, mpTracker->GetCurrentPose());
	}

	//      mGLWindow.GetMousePoseUpdate();
	string sCaption;
	if(bDrawMap)
		sCaption = mpMapViewer->GetMessageForUser();
	else
		sCaption = mpTracker->GetMessageForUser();
	mGLWindow.DrawCaption(sCaption);
	mGLWindow.DrawMenus();
	mGLWindow.swap_buffers();
	mGLWindow.HandlePendingEvents();
	}
}

void System::GUICommandCallBack(void *ptr, string sCommand, string sParams)
{
  if(sCommand=="quit" || sCommand == "exit")
    static_cast<System*>(ptr)->mbDone = true;
}








