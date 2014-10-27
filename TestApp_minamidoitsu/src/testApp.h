#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxPostGlitch.h"

#include "BaseContentsInterface.h"

// input mode
//#define USE_OF_AUDIO_IN
#define USE_OSC_RECEIVER

#define MAIN_LEVEL  mLevel
#define MAIN_WAVE   left

#define CONTENT_LIST_TYPE vector
#define VIDEO_TYPE ofVideoPlayer

static const int RECEIVE_PORT = 9000;

class testApp : public ofBaseApp
{
	
public:
	
	void setup();
	void update();
	void draw();
    void exit();
	void keyPressed(int key);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    
private:
    
    void drawInputSoundStates();
    void addContentsSwitch(const int add);
    void removeContentsSwitch(const int rm);
    void toggleContentsSwitch(const int toggle);
    bool checkUnique(const int num);
    void sendBang();
    void generateWave(WAVE_TYPE & wave);
    void generateNoise();
    void stopNoise();
    
    
    // syphone
	ofxSyphonServer mServer;
	ofxSyphonClient mClient;
    ofFbo mMainFbo;
    
    // contents
    CONTENT_LIST_TYPE<shared_ptr<BaseContentsInterface> > mContents;
    typedef CONTENT_LIST_TYPE<shared_ptr<BaseContentsInterface> >::iterator content_it;
    deque<int> mContentsSwitches;
    
    // sount input
    ofSoundStream soundStream;
    WAVE_TYPE left;
    WAVE_TYPE right;
    vector <float> volHistory;
    int 	bufferCounter;
    int 	drawCounter;
    float smoothedVol;
    float scaledVol;
    
    // osc
    ofxOscReceiver mReceiver;
    
    // gui
    ofxPanel mGuiPanel;
    ofParameterGroup mParamGroup;
    ofParameter<float> mLevel;
    ofParameter<float> mGain;
    ofParameter<float> mSmoothLevel;
    ofParameter<bool> bDrawInputSoundStates;
    ofParameter<bool> bVideo;
    ofParameter<int> mSelVideo;
    ofParameter<bool> bNoise;
    
    // post glitch
    ofxPostGlitch mPostGlitch;
    int mGlitchTime;
    int mGlitchCount;
    
    // textures
//    Media media;
    ofImage mLogo;
    
    ofShader shader;
//    vector<ofImage> mImages;
//    vector<VIDEO_TYPE> mVideos;
//    ofTexture mTex;

};

#endif

