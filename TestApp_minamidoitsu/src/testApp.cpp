#include "testApp.h"
#include "PictureSlideShow.hpp"
#include "GeometWave.hpp"
#include "KzdPatternExample.hpp"
#include "RotationSphere.hpp"
#include "Orientation.hpp"
#include "RotationCube.hpp"
#include "BoxPerticle.hpp"

#define FOR_SWITCHES for (deque<int>::iterator it = mContentsSwitches.begin(); it != mContentsSwitches.end(); it++)

void testApp::setup()
{
    ofSetWindowShape(common::width, common::height);
	ofSetCircleResolution(60);
    
    //===============================================
    // setup syphon
    //===============================================
	mServer.setName("Screen Output");
	mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("","Simple Server");
	   
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    
    ofFbo::Settings s;
    s.width = WIDTH;
    s.height = HEIGHT;
    s.internalformat = GL_RGBA;
    s.useDepth = true;
    s.useStencil = true;
    
    mMainFbo.allocate(s);
    
    
    //===============================================
    // sound input
    //===============================================
#ifdef USE_OF_AUDIO_IN
    ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);
	
	// 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
	soundStream.listDevices();
	
	//if you want to set a different device id
	soundStream.setDeviceID(common::soundDeviceID); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
	int bufferSize = common::bufferSize;
	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
#endif
    
#ifdef USE_OSC_RECEIVER
    mReceiver.setup(RECEIVE_PORT);
#endif
    
    if (MAIN_WAVE.empty()) MAIN_WAVE.assign(256, 0.0);
    

    
    //===============================================
    // setup ofxPostGlitch
    //===============================================
    mPostGlitch.shader[0].load("Shaders/convergence");
    mPostGlitch.shader[1].load("Shaders/glow");
    mPostGlitch.shader[2].load("Shaders/shaker");
    mPostGlitch.shader[3].load("Shaders/cut_slider");
    mPostGlitch.shader[4].load("Shaders/twist");
    mPostGlitch.shader[5].load("Shaders/outline");
    mPostGlitch.shader[6].load("Shaders/noise");
    mPostGlitch.shader[7].load("Shaders/slitscan");
    mPostGlitch.shader[8].load("Shaders/swell");
    mPostGlitch.shader[9].load("Shaders/invert");
    mPostGlitch.shader[10].load("Shaders/crHighContrast");
    mPostGlitch.shader[11].load("Shaders/crBlueraise");
    mPostGlitch.shader[12].load("Shaders/crRedraise");
    mPostGlitch.shader[13].load("Shaders/crGreenraise");
    mPostGlitch.shader[14].load("Shaders/crRedinvert");
    mPostGlitch.shader[15].load("Shaders/crBlueinvert");
    mPostGlitch.shader[16].load("Shaders/crGreeninvert");
    mPostGlitch.setup(&mMainFbo);
    mGlitchCount = 0;
    mGlitchTime = 10;
    

    //===============================================
    // load texture resouces
    //===============================================
//    if (!media.loadImages("pictures")) OF_EXIT_APP(1);
//    if (!media.loadVideos("videos")) OF_EXIT_APP(1);
//    shuffleTexture();
    mLogo.loadImage("logo.png");
    
    //===============================================
    // create contents list
    //===============================================
    mContents.push_back(shared_ptr<BaseContentsInterface>(new PictureSlideShow(mLogo.getTextureReference())));
    mContents.push_back(shared_ptr<BaseContentsInterface>(new GeometWave()));
    mContents.push_back(shared_ptr<BaseContentsInterface>(new KzdPatternExample()));
    mContents.push_back(shared_ptr<BaseContentsInterface>(new RotationSphere()));
    mContents.push_back(shared_ptr<BaseContentsInterface>(new Orientation()));
    mContents.push_back(shared_ptr<BaseContentsInterface>(new RotationCube(mLogo.getTextureReference())));
    mContents.push_back(shared_ptr<BaseContentsInterface>(new BoxPerticle(mLogo.getTextureReference(), 200)));
    
    for (content_it it = mContents.begin(); it != mContents.end(); it++) {
        (*it)->updateSoundStatus(&MAIN_WAVE, MAIN_LEVEL);
        (*it)->setupGui();
        (*it)->setup();
    }
    
    
    //===============================================
    // setup GUI
    //===============================================
    mGuiPanel.setName("GUI");
    mParamGroup.setName("PARAMETERS");
    mParamGroup.add(mLevel.set("level", 0.0, 0.0, 1.0));
    mParamGroup.add(mGain.set("gain", 1.0, 0.0, 10.0)); 
#ifdef USE_OF_AUDIO_IN
    mParamGroup.add(mSmoothLevel.set("smooth_level", 0.8, 0.0, 1.0));
#endif
//    mParamGroup.add(bDrawInputSoundStates.set("show_input_status", false));
//    mParamGroup.add(bVideo.set("video_mode", false));
//    mParamGroup.add(mSelVideo.set("select_video", 0, 0, media.mVideos.size() - 1));
    mParamGroup.add(bNoise.set("noise", false));
    
    mGuiPanel.setup(mParamGroup);
    for (content_it it = mContents.begin(); it != mContents.end(); it++) {
        if ((*it)->getParamGroup().size() > 0 && !(*it)->getName().empty()) {
            mGuiPanel.add((*it)->getParamGroup());
        }
    }
    mGuiPanel.minimizeAll();
    
    mGuiPanel.loadFromFile("settings.xml");

}

void testApp::update()
{
    //===============================================
    // update share values
    //===============================================
    share::elapsedTime = ofGetElapsedTimef();
    
//    media.bVideo = bVideo;
//    media.mSelVideo = mSelVideo;
//    media.update();
    
    
    //===============================================
    // sound input update
    //===============================================
#ifdef USE_OF_AUDIO_IN
    //lets scale the vol up to a 0-1 range
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    // add gain
    MAIN_LEVEL *= mGain;
    MAIN_LEVEL = bClipping ? ofClamp(MAIN_LEVEL, 0.0, 1.0) : MAIN_LEVEL;
    
	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
#else
    
    //-----------------
    // create wave
    //-----------------
    generateWave(MAIN_WAVE);
    
#endif
    
#ifdef USE_OSC_RECEIVER
    bool didSendBang = false;
    while (mReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
        if (m.getAddress() == "/level") {
            switch (m.getArgType(0)) {
                case OFXOSC_TYPE_INT32: mLevel = m.getArgAsInt32(0); break;
                case OFXOSC_TYPE_FLOAT: mLevel = m.getArgAsFloat(0); break;
                default: cout << "osc err" << endl; break;
            }
        }
        if (m.getAddress() == "/bang" && !didSendBang) {
            sendBang();
            didSendBang = true;
        }
    }
    MAIN_LEVEL *= mGain;
#endif
    
    //===============================================
    // contents update
    //===============================================
    float tmpLv = MAIN_LEVEL;
    FOR_SWITCHES {
        mContents[*it]->updateSoundStatus(&MAIN_WAVE, tmpLv);
        mContents[*it]->update();
    }
    
    
    if (mGlitchCount > 0) {
        mGlitchCount--;
    } else {
        stopNoise();
    }
}

void testApp::draw()
{

    //===============================================
    // content draw
    //===============================================
    mMainFbo.begin();
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    FOR_SWITCHES { mContents[*it]->draw(); }
    mMainFbo.end();
    
    mPostGlitch.generateFx();
    mMainFbo.draw(0, 0);
    
    //===============================================
    // sound input draw
    //===============================================
#ifdef USE_OF_AUDIO_IN
    ofSetColor(255, 255, 255);
    if (bDrawInputSoundStates) drawInputSoundStates();
#endif
    
    //===============================================
    // gui draw
    //===============================================
    mGuiPanel.draw();
    
    //===============================================
    // infomation text
    //===============================================
    stringstream s;
    s << "fps: " << ofGetFrameRate() << endl;
    s << "num contents: " << countContents << endl;
    s << "enable: ";
    FOR_SWITCHES { s << *it << " "; } s<< endl;
    ofDrawBitmapString(s.str(), mGuiPanel.getPosition().x, mGuiPanel.getPosition().y + mGuiPanel.getHeight() + 20);
    
    // publish syphone server
    mServer.publishTexture(&mMainFbo.getTextureReference());
    
}

void testApp::exit()
{
    mGuiPanel.saveToFile("settings.xml");
}

void testApp::drawInputSoundStates()
{
    ofSetColor(225);
	ofDrawBitmapString("AUDIO INPUT EXAMPLE", 32, 32);
	
	ofNoFill();
	
	// draw the left channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Left Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (unsigned int i = 0; i < left.size(); i++){
        ofVertex(i*2, 100 -left[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
    
	// draw the right channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 370, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Right Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (unsigned int i = 0; i < right.size(); i++){
        ofVertex(i*2, 100 -right[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
	
	// draw the average volume:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(565, 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    ofRect(0, 0, 400, 400);
    
    ofSetColor(245, 58, 135);
    ofFill();
    ofCircle(200, 200, scaledVol * 190.0f);
    
    //lets draw the volume history as a graph
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        if( i == 0 ) ofVertex(i, 400);
        
        ofVertex(i, 400 - volHistory[i] * 70);
        
        if( i == volHistory.size() -1 ) ofVertex(i, 400);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
	
	drawCounter++;
	
	ofSetColor(225);
	string reportString = "buffers received: "+ofToString(bufferCounter)+"\ndraw routines called: "+ofToString(drawCounter)+"\nticks: " + ofToString(soundStream.getTickCount());
	ofDrawBitmapString(reportString, 32, 589);
}

void testApp::addContentsSwitch(const int add)
{
    if (checkUnique(add)) {
        mContentsSwitches.push_back(add);
    }
}

void testApp::removeContentsSwitch(const int rm)
{
    FOR_SWITCHES {
        if (rm == *it) {
            mContentsSwitches.erase(it);
            break;
        }
    }
}

void testApp::toggleContentsSwitch(const int toggle)
{
    if (checkUnique(toggle)) {
        mContentsSwitches.push_back(toggle);
    } else {
        removeContentsSwitch(toggle);
    }
}

bool testApp::checkUnique(const int num)
{
    if (num >= mContents.size()) return false;
    FOR_SWITCHES {
        if (num == *it) return false;
    }
    return true;
}

void testApp::sendBang()
{
    FOR_SWITCHES mContents[*it]->getBang();
    if (bNoise) generateNoise();
}

void testApp::generateWave(WAVE_TYPE &wave)
{
//    float y = 50;
//    for (int i = 0; i < MAIN_WAVE.size(); i++) {
//        MAIN_WAVE[i] = ofRandomf() * MAIN_LEVEL;
//        MAIN_WAVE[i] = ofNoise(i, ofGetElapsedTimef()) * MAIN_LEVEL;
//        MAIN_WAVE[i] = ofSignedNoise(i + ofGetElapsedTimef()) * MAIN_LEVEL;
//    }

    float seed = ofRandom(10);
    for (int i = 0; i <= MAIN_WAVE.size(); i++) {
//        MAIN_WAVE[i] = ofNoise(seed);
//        seed += 0.01;
        MAIN_WAVE[i] = ofRandomf() * MAIN_LEVEL;
    }
    
}

void testApp::generateNoise()
{
    stopNoise();
    
    int rnd = ofRandom(8);
    switch (rnd) {
        case 0: mPostGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true); break;
        case 1: mPostGlitch.setFx(OFXPOSTGLITCH_SHAKER, true); break;
        case 2: mPostGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true); break;
        case 3: mPostGlitch.setFx(OFXPOSTGLITCH_TWIST, true); break;
        case 4: mPostGlitch.setFx(OFXPOSTGLITCH_OUTLINE, true); break;
        case 5: mPostGlitch.setFx(OFXPOSTGLITCH_NOISE, true); break;
        case 6: mPostGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true); break;
        case 7: mPostGlitch.setFx(OFXPOSTGLITCH_SWELL, true); break;
    }
    mGlitchTime = ofRandom(5, 20);
    mGlitchCount = mGlitchTime;
}

void testApp::stopNoise()
{
    for (int i = 0; i < GLITCH_NUM; i++) {
        if (mPostGlitch.bShading[i]) {
            mPostGlitch.bShading[i] = false;
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  KEY PRESSED                                                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void testApp::keyPressed(int key)
{
    switch (key) {
        case 'f': ofToggleFullscreen(); break;
        case ' ': bDrawInputSoundStates = !bDrawInputSoundStates; break;

        case '0': toggleContentsSwitch(0); break;
        case '1': toggleContentsSwitch(1); break;
        case '2': toggleContentsSwitch(2); break;
        case '3': toggleContentsSwitch(3); break;
        case '4': toggleContentsSwitch(4); break;
        case '5': toggleContentsSwitch(5); break;
        case '6': toggleContentsSwitch(6); break;
        case '7': toggleContentsSwitch(7); break;
        case '8': toggleContentsSwitch(8); break;
        case '9': toggleContentsSwitch(9); break;
            
        case 'v': bVideo ^= true; break;
        case '=': generateNoise(); break;
        case 'b': sendBang(); break;
        
        default: FOR_SWITCHES mContents[*it]->keyPressed(key); break;
    }
    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  AUDIO IN                                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void testApp::audioIn(float * input, int bufferSize, int nChannels)
{
#ifdef USE_OF_AUDIO_IN
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        left[i] *= mGain;
        right[i] *= mGain;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= mSmoothLevel;
	smoothedVol += (-mSmoothLevel + 1) * curVol;
	
	bufferCounter++;
#endif
}
