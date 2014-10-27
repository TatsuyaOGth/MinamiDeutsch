#pragma once

#include "ofMain.h"


class BaseAnimationUnit
{
protected:
    typedef BaseAnimationUnit base;
    bool bDied;

    double getLastFrameTime() { return ofGetLastFrameTime() * 1000; }
    
public:
    BaseAnimationUnit():bDied(false){}
    
    virtual void update() = 0;
    virtual void draw() = 0;
    
    bool isDied() { return bDied; }
};



class Perticle : public BaseAnimationUnit
{
    const ofVec2f mPos;
    const float mLife;
    float mVecX;
    float mVecY;
    float mX;
    float mY;
    const float mAccel;
    float mDecey;
    float mSize;
    
public:
    Perticle(float x, float y, float vecX, float vecY, float size, float accel, float life):
    mPos(ofVec2f(x, y)),
    mLife(life),
    mVecX(vecX),
    mVecY(vecY),
    mSize(size),
    mAccel(accel)
    {
        mX = mPos.x;
        mY = mPos.y;
        mDecey = mLife;
    }
    
    void update()
    {
        mX += mVecX;
        mY += mVecY;
        mVecX *= mAccel;
        mVecY *= mAccel;
        mDecey -= base::getLastFrameTime();
        if (mDecey < 0) bDied = true;
    }
    
    void draw()
    {
        if (mDecey > mLife * 0.3) {
            ofCircle(mX, mY, mSize);
        } else {
            // blink
            float rand = ofRandomuf();
            if (rand > 0.3) {
                ofCircle(mX, mY, mSize);
            }
        }
    }
};

