#pragma once

#include "ofMain.h"
#include "ofxEWCLIB.h"

class ofApp : public ofBaseApp {
private:
	ofxEWCLIB ewc;
	ofVideoGrabber grabber;

public:
	void setup();
	void update();
	void draw();
	void keyReleased(int key);

};
