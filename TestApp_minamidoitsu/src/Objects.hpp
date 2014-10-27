#pragma once

#include "../../common.h"

class slide : public ofPoint{
private:
    
    ofPoint p;
    
    float speed;
    
public:
    
    slide():speed(0.1){
    }
    
    void set(float _px,float _py,float _pz){
        p.x = _px;
        p.y = _py;
        p.z = _pz;
    }
    
    void set(float _px,float _py){
        p.x = _px;
        p.y = _py;
    }
    
    void imSet(float _px,float _py,float _pz){
        p.x = _px;
        p.y = _py;
        p.z = _pz;
        
        x = _px;
        y = _py;
        z = _pz;
    }
    
    void imSet(float _px,float _py){
        p.x = _px;
        p.y = _py;
        
        x = _px;
        y = _py;
        
    }
    
    void update(){
        x += (p.x - x) * speed;
        y += (p.y - y) * speed;
        z += (p.z - z) * speed;
    }
};

class PrimitiveObject
{
public:
    
    ofPoint pos;
    ofVec3f size;
    ofVec3f rotate;
    ofColor color;
    int drawMode;
    bool bFill;
    float audioVal;
    int rotAxisMode;
    
    PrimitiveObject():bFill(false),rotAxisMode(1){
        color.set(255,255,255,255);
        size.set(10,10);
        drawMode = 0;
        pos.set(WIDTH * 0.5 ,HEIGHT * 0.5);
    }
    void update(){
        float max = WIDTH * 0.2;
        size.set(audioVal * max, audioVal * max);
    }
    
    void draw(){
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetSphereResolution(2);
        if(bFill)ofFill();
        else ofNoFill();
        
        ofSetColor(color.r,color.g,color.b,color.a);
        
        if(rotAxisMode == 1)ofTranslate(pos.x,pos.y,pos.z);
        else if(rotAxisMode == 0)ofTranslate(WIDTH * 0.5, HEIGHT * 0.5);
        
        ofRotateX(rotate.x);
        ofRotateY(rotate.y);
        ofRotateZ(rotate.z);
        
        if(rotAxisMode == 0){
            ofTranslate(-WIDTH * 0.5, -HEIGHT * 0.5);
            ofTranslate(pos.x,pos.y,pos.z);
        }
        if(drawMode == 0) ofDrawBox(0,0,0,size.x,size.y,size.z);
        else ofDrawSphere(0,0,0,size.x * 0.5);
        
        ofPopStyle();
        ofPopMatrix();
        
    }
};


class RotationArc
{
    int length;
    float deg;
    
public:
    ofVec2f pos;
    ofVec2f pts;
    float size;
    float rot;
    
    RotationArc(int x, int y, int _length, float _size, float rotation):
    pos(ofVec2f(x, y)),
    size(_size),
    rot(rotation)
    {
        setLength(_length);
        deg = 0;
    }
    
    void update()
    {
        deg += rot;
    }
    
    void draw()
    {
        ofPushMatrix();
        ofTranslate(pos);
        ofRotate(deg, 0, 0, 1);
        
        ofBeginShape();
        for (unsigned int i = 0; i < length; i++) {
            float c = ((float)i / 360) * TWO_PI;
            float x = sin(c) * size;
            float y = cos(c) * size;
            ofVertex(x, y);
        }
        ofEndShape(false);
        
        ofPopMatrix();
    }
    
    void setLength(int v)
    {
        length = ofClamp(v, 0, 360);
    }
};

