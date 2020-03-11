#include "ofApp.h"

//#define TEST

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	int w = 1920;
	int h = 1080;
	ofSetWindowShape(w, h);

#ifndef TEST
	//--- print formats of connected cameras
	ewc.printAllFormats();

	//--- setup by cameraID
	//ewc.setup(1, w,h);

	//--- setup by deviceName
	ewc.setup("C920", w, h);

#else
	grabber.setDeviceID(1);
	grabber.setup(w,h);
#endif
}
//--------------------------------------------------------------
void ofApp::update(){
#ifndef TEST
	ewc.update();
#else
	grabber.update();
#endif
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifndef TEST
	ewc.draw(0,0);
#else
	grabber.draw(0, 0);
#endif

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == ' ') ewc.openProperty();
	else if (key == 'f') ofToggleFullscreen();
}