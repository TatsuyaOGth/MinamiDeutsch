#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetWindowTitle("Syphon Server");
    ofSetWindowShape(common::width, common::height);
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    
	dir.setup();
    client.setup();
    
    ofAddListener(dir.events.serverAnnounced, this, &testApp::serverAnnounced);
    
    // not yet implemented
    //ofAddListener(dir.events.serverUpdated, this, &testApp::serverUpdated);
    ofAddListener(dir.events.serverRetired, this, &testApp::serverRetired);

    dirIdx = -1;
    
    _mapping = new ofxMtlMapping2D();
    _mapping->init(common::width, common::height, "mapping/xml/shapes.xml", "mapping/controls/mapping.xml");
    
    bMapping = true;
}

//these are our directory's callbacks
void testApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void testApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void testApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

//--------------------------------------------------------------
void testApp::update()
{
    _mapping->update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    if (bMapping)
    {
        _mapping->bind();
        ofBackground(0, 0, 0);
        ofSetColor(255, 255, 255, 255);
        if(dir.isValidIndex(dirIdx)) client.draw(0, 0);
        _mapping->unbind();
        _mapping->draw();
    }
    else
    {
        ofBackground(0, 0, 0);
        ofSetColor(255, 255, 255, 255);
        if(dir.isValidIndex(dirIdx)) client.draw(0, 0);
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    switch (key) {
        case ' ': changeClient(); break;
        case 'f': ofToggleFullscreen(); break;
        case 'M': bMapping = !bMapping; break;
    }
}

void testApp::changeClient()
{
    dirIdx++;
    if(dirIdx > dir.size() - 1)
        dirIdx = 0;
    
    if (dir.size() == 0) {
        ofLogError() << "null clients";
        return;
    }
    client.set(dir.getDescription(dirIdx));
    string serverName = client.getServerName();
    string appName = client.getApplicationName();
    
    if(serverName == ""){
        serverName = "null";
    }
    if(appName == ""){
        appName = "null";
    }
    ofSetWindowTitle(serverName + ":" + appName);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}