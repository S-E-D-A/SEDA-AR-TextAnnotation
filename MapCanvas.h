// Stores information for a 6 DOF "canvas". A canvas
// is a plane on which AR graphics are drawn


#ifndef __MAP_CANVAS_H
#define __MAP_CANVAS_H

#include <TooN/TooN.h>
using namespace TooN;

struct MapCanvas
{

    SE3<> se3CFromW; //The coordinate frame as a Camera-from-world Transform


}


#endif
