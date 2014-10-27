#pragma once

#include "BaseContentsInterface.h"

class PictureSlideShow : public BaseContentsInterface
{
    ofTexture & mTex;
    
    ofParameter<bool> gray;
    
public:
    PictureSlideShow(ofTexture & tex):
    mTex(tex)
    {
    }
    
    void setup()
    {
        
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        if (mTex.isAllocated()) {
            ofEnableAlphaBlending();
            mTex.draw(0, 0, WIDTH, HEIGHT);
            ofDisableAlphaBlending();
        }
    }
    
    void getBang()
    {
        
    }
    
};
