#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "AnimationUnits.hpp"
#include "Objects.hpp"
#include "../../common.h"

#define LEVEL   getLevel()
#define WAVE    getWave()

static int countContents = 0;

class BaseContentsInterface
{
public:
    BaseContentsInterface():mLevel(0),mWave(0)
    {
        countContents++;
    }
    virtual ~BaseContentsInterface()
    {
        countContents--;
    }
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}
    
    virtual void windowResized(int w, int h){}
    
    virtual void keyPressed( int key ){}
    virtual void keyReleased( int key ){}
    
    virtual void mouseMoved( int x, int y ){}
    virtual void mouseDragged( int x, int y, int button ){}
    virtual void mousePressed( int x, int y, int button ){}
    virtual void mouseReleased(int x, int y, int button ){}
    
    virtual void dragEvent(ofDragInfo dragInfo) { }
    virtual void gotMessage(ofMessage msg){ }
    
    // update sound status
    void updateSoundStatus(WAVE_TYPE * wave, LEVEL_TYPE level)
    {
        mWave = wave;
        mLevel = level;
    }
    
    virtual void getBang(){}
    
    inline string& getName()
    {
        if (mName.empty()) {
            string ret = "";
            const type_info& id = typeid(*this);
            int stat;
            char *name = abi::__cxa_demangle(id.name(), 0, 0, &stat);
            if( stat == 0) {
                mName += name;
            }
            free(name);
        }
        return mName;
    }
    
    void setupGui()
    {
        mParamGroup.setName(getName());
    }
    
    ofParameterGroup getParamGroup() { return mParamGroup; }

protected:
    WAVE_TYPE *     getWavePtr() const { return  mWave; }
    WAVE_TYPE       getWave() const { return  *mWave; }
    LEVEL_TYPE      getLevel() const { return mLevel; }
    
    float getWidth() { return WIDTH; }
    float getHeight() { return HEIGHT; }
    float getOpacity() { return 1.0; }
    
    
    typedef BaseContentsInterface base;
    typedef list<shared_ptr<BaseAnimationUnit> >::iterator anm_it;
    
    ofParameterGroup mParamGroup;
    
    template<class T>
    void setParameter(ofParameter<T> & param) {
        mParamGroup.add(param);
    }
    
private:
    LEVEL_TYPE mLevel;
    WAVE_TYPE  * mWave;
    string mName; ///< this class name
};


///////////////////////////////////////////////////////////////
// template
///////////////////////////////////////////////////////////////
/*

 
#pragma once
 
#include "BaseContentsInterface.h"

class <# class name #> : public BaseContentsInterface
{
    
public:
    <# class name #>()
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
        
    }
    
    void getBang()
    {

    }
    
};

*/