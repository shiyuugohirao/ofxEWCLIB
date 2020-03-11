#pragma once
#include "ofMain.h"
#include <uuids.h>

class ofxEWCLIB {
	map<int, int> lastCounts;	// <cameraID, count>
	map<int, ofImage> images;	// <cameraID, ofImage>
public:
	ofxEWCLIB();
	~ofxEWCLIB();
	void setup(int cameraID, int width, int height, double fps = 30.0, GUID device_type = MEDIASUBTYPE_MJPG);
	void setup(string deviceName, int width, int height, double fps = 30.0, GUID device_type = MEDIASUBTYPE_MJPG);
	void update(int cameraID = -1);
	void draw(int x, int y, int cameraID = -1);
	bool isInitialized(int cameraID = -1) const;
	bool isCaptured(int cameraID) const;
	bool isFrameNew(int cameraID = -1);

	void start(int cameraID = -1);
	void stop(int cameraID = -1);
	void pause(int cameraID = -1);
	void close(int cameraID = -1);

	float getSampleTime(int cameraID = -1);
	float getFrameTime(int cameraID = -1);
	float getFps(int cameraID = -1);
	int getCount(int cameraID = -1);

	void printFormat(int cameraID = -1);

	//--- use before setup()
	void printAllFormats();

	//--- have to be after EWC_OPEN()
	int getCameraNum();

	//--- must be after EWC_OPEN(num)
	string getDeviceName(int num);


	ofImage getImage(int cameraID = -1);
	ofPixels getPixels(int cameraID = -1);
	ofTexture getTexture(int cameraID = -1);

	bool openProperty(int cameraID = -1);

	//--- TODO ---//
	//void addCamera(int cameraID, int width, int height, double fps = 30.0, GUID device_type = MEDIASUBTYPE_MJPG);

	//- CameraSetting
	//double EWC_GetValue(int num, int prop, int *mode);
	//int EWC_SetValue(int num, int prop, double value);
	//int EWC_SetDefault(int num, int prop);
	//int EWC_SetAuto(int num, int prop);
	//int EWC_SetManual(int num, int prop);

	//int EWC_SaveProperty(int num, char *filename = NULL);
	//int EWC_LoadProperty(int num, char *filename = NULL);
};
