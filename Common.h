//--------------------------------------------------------------------------------------
// Commonly used definitions across entire project
//--------------------------------------------------------------------------------------
#ifndef _COMMON_H_INCLUDED_
#define _COMMON_H_INCLUDED_

#include <windows.h>
#include <d3d11.h>
#include <string>

#include "CVector3.h"
#include "CMatrix4x4.h"


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
// This global variables are like small config file, so getting rid of them seems useles.
// All deffinitions of this variables are in main.cpp except gLasrError which is used like
// runtime error trace, which is pretty usefull

// Windows variables
extern HWND gHWnd;

// Viewport size
extern int gViewportWidth;
extern int gViewportHeight;

// Constants controlling speed of movement/rotation (measured in units per second because we're using frame time)
extern const float ROTATION_SPEED;  // 2 radians per second for rotation
extern const float MOVEMENT_SPEED; // 50 units per second for movement (what a unit of length is depends on 3D model - i.e. an artist decision usually)

// A global error message to help track down fatal errors - set it to a useful message
// when a serious error occurs
extern std::string gLastError;

struct PerFrameGlobalConstants
{
    CMatrix4x4 viewMatrix;
    CMatrix4x4 projectionMatrix;
    CMatrix4x4 viewProjectionMatrix;
};

// This is the matrix that positions the next thing to be rendered in the scene. Unlike the structure above this data can be
// updated and sent to the GPU several times every frame (once per model). However, apart from that it works in the same way.
struct PerModelConstants
{
    CMatrix4x4 worldMatrix;
    CVector3   objectColour; // Allows each light model to be tinted to match the light colour they cast
    float      padding6;
};

#endif //_COMMON_H_INCLUDED_
