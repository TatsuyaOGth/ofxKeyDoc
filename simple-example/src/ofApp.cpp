#include "ofApp.h"
#include "ofxKeyDoc.h"

float x;
float y;
ofColor col;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofxKeyDoc::read(__FILE__);
    //ofxKeyDoc::read(__FILE__, false);
    
    ofxKeyDoc::saveDoc();

    x = ofGetWidth()/2;
    y = ofGetHeight()/2;
    col.set(255, 0, 255);
    ofSetRectMode(OF_RECTMODE_CENTER);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(30);
    ofSetColor(col);
    
    ofRect(x, y, 20, 20);
    
    ofSetColor(255);
    ofDrawBitmapString(ofxKeyDoc::getDoc(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case OF_KEY_LEFT: /// move left
            x -= 10;
            break;
            
        case OF_KEY_RIGHT: /// move right
            x += 10;
            break;
            
        case OF_KEY_UP: y   -= 10;  break; /// move up
        case OF_KEY_DOWN: y += 10;  break; /// move down
    }
    
    if (key == '1') { /// set red
        col.set(255, 0, 0);
    }
    
    if (key == '2') col.set(0, 255, 0); /// set green
    
    if (key == '3' || key == '4') { /// set blue
        col.set(0, 0, 255);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
