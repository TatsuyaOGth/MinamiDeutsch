#pragma once

//==============================================================
// common parameters
//==============================================================
namespace common
{
    // main window size
    static const float width = 1200;
    static const float height = 800;
    
    // sound devise
    static const int soundDeviceID = 3;
    static const int bufferSize = 256;
    
    // glitch time
    static const int glitchTime = 10;// frame
}

namespace share
{
    static float elapsedTime = 0;
}

#define WIDTH   common::width
#define HEIGHT  common::height


//==============================================================
// share types
//==============================================================
typedef vector<float>   WAVE_TYPE;
typedef float           LEVEL_TYPE;



#define debugMouseX(a) (((float)ofGetMouseX()/(float)ofGetWidth())*a)
#define debugMouseY(a) (((float)ofGetMouseY()/(float)ofGetHeight())*a)