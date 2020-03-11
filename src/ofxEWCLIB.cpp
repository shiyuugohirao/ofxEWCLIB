#include "ofxEWCLIB.h"
#include "ewclib.h"

ofxEWCLIB::ofxEWCLIB() {

}
ofxEWCLIB::~ofxEWCLIB() {
	close();
}

//--------------------------------------------------------------
void ofxEWCLIB::setup(int cameraID, int width, int height, double fps, GUID device_type) {

	ewc_device_type = device_type;
	int ret = EWC_Open(cameraID, width, height, fps, cameraID, MEDIASUBTYPE_RGB24);

	if (ret != 0) {
		fprintf(stderr, "EWC_Open failed.(%d: %d x %d)\n", cameraID, width, height);
		return;
	}
	else {
		ofLogNotice("ofxEWCLIB") << "camera open! (" << getDeviceName(cameraID) << ")";
	}
	//EWC_SetValue(cameraID, EWC_FOCUS, 0.0);
	lastCounts.insert(make_pair(cameraID, 0));
	images.insert(make_pair(cameraID, ofImage()));
	images[cameraID].allocate(width, height, OF_IMAGE_COLOR);
	images[cameraID].getPixels().swapRgb();
	EWC_SetBuffer(cameraID, images[cameraID].getPixels().getData());
}
//--------------------------------------------------------------
void ofxEWCLIB::setup(string deviceName, int width, int height, double fps, GUID device_type) {
	int cameraID = images.size() + 1;
	ewc_device_type = device_type;

	// deviceName is checked with strstr()
	// WindowsDeviceManager > ImageingDevices 
	int ret = EWC_Open(cameraID, width, height, fps, &deviceName[0], MEDIASUBTYPE_RGB24);

	if (ret != 0) {
		char subtype[30];
		EWC_GUIDtoTEXT(ewc_device_type, subtype, sizeof(subtype));
		fprintf(stderr, "EWC_Open failed.(%s: %d x %d) %6.3f[fps] %s\n", deviceName.c_str(), width, height, fps, subtype);
		return;
	}
	else {
		ofLogNotice("ofxEWCLIB") << "camera open! (" << deviceName << ")";
	}
	//EWC_SetValue(cameraID, EWC_FOCUS, 0.0);
	lastCounts.insert(make_pair(cameraID, 0));
	images.insert(make_pair(cameraID, ofImage()));
	images[cameraID].allocate(width, height, OF_IMAGE_COLOR);
	images[cameraID].getPixels().swapRgb();
	EWC_SetBuffer(cameraID, images[cameraID].getPixels().getData());

}
//--------------------------------------------------------------
void ofxEWCLIB::update(int cameraID) {
	if (cameraID < 0) {
		for (int i = 0; i < images.size(); i++) {
			cameraID = std::next(images.begin(), i)->first;
			if (EWC_IsCaptured(cameraID)) {
				images[cameraID].update();
			}
		}
	}
	else {
		if (EWC_IsCaptured(cameraID)) {
			images[cameraID].update();
		}
	}
}
//--------------------------------------------------------------
void ofxEWCLIB::draw(int x, int y, int cameraID) {
	cameraID = (cameraID < 0) ? lastCounts.begin()->first : cameraID;
	images[cameraID].draw(x, y);
}

//--------------------------------------------------------------
bool  ofxEWCLIB::isInitialized(int cameraID) const {
	cameraID = (cameraID < 0) ? lastCounts.begin()->first : cameraID;
	return bool(EWC_GetBufferSize(cameraID) > 0);
}
//--------------------------------------------------------------
bool  ofxEWCLIB::isCaptured(int cameraID) const {
	cameraID = (cameraID < 0) ? lastCounts.begin()->first : cameraID;
	return EWC_IsCaptured(cameraID);
}
//--------------------------------------------------------------
bool  ofxEWCLIB::isFrameNew(int cameraID) {
	cameraID = (cameraID < 0) ? lastCounts.begin()->first : cameraID;
	int currentCount = getCount(cameraID);
	if (currentCount > lastCounts[cameraID]) {
		lastCounts[cameraID] = currentCount;
		return true;
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------
void ofxEWCLIB::start(int cameraID) {
	if (cameraID < 0) {
		for (int i = 0; i < images.size(); i++) {
			cameraID = std::next(images.begin(), i)->first;
			EWC_Run(cameraID);
		}
	}
	else {
		EWC_Run(cameraID);
	}
}
//--------------------------------------------------------------
void ofxEWCLIB::stop(int cameraID) {
	if (cameraID < 0) {
		for (int i = 0; i < images.size(); i++) {
			cameraID = std::next(images.begin(), i)->first;
			EWC_Stop(cameraID);
		}
	}
	else {
		EWC_Stop(cameraID);
	}
}
//--------------------------------------------------------------
void ofxEWCLIB::pause(int cameraID) {
	if (cameraID < 0) {
		for (int i = 0; i < images.size(); i++) {
			cameraID = std::next(images.begin(), i)->first;
			EWC_Pause(cameraID);
		}
	}
	else {
		EWC_Pause(cameraID);
	}
}
//--------------------------------------------------------------
void ofxEWCLIB::close(int cameraID) {
	if (cameraID < 0) {
		EWC_CloseAll();
	}
	else {
		EWC_Close(cameraID);
	}
}

//--------------------------------------------------------------
void ofxEWCLIB::printFormat(int cameraID) {
	const int MAXFORMAT = 100;
	ewc_format f[MAXFORMAT];
	int n = MAXFORMAT;
	if (EWC_GetFormat(cameraID, f, &n)) return;

	for (int i = 0; i < n; i++) {
		printf("%3d: %5d x%5d(%2dbit)  %6.3f[fps] %s\n"
			, i, f[i].width, f[i].height, f[i].bit, f[i].fps, f[i].subtype_t);
	}
}
//--------------------------------------------------------------
void ofxEWCLIB::printAllFormats() {
	for (int i = 0; i < 10; i++) {
		int ret = EWC_Open(i, 0, 0, 0, i);
		if (ret != 0) break;

		printf("------------------------------\n");
		printf("LogicalCameraID:%3i - %s\n", i, getDeviceName(i).c_str());
		printFormat(i);
		EWC_Close(i);
	}
}


//--------------------------------------------------------------
ofImage ofxEWCLIB::getImage(int cameraID) {
	return (cameraID < 0) ? images[lastCounts.begin()->first] : images[cameraID];
}
ofPixels ofxEWCLIB::getPixels(int cameraID) {
	return getImage(cameraID).getPixels();
}
ofTexture ofxEWCLIB::getTexture(int cameraID) {
	return getImage(cameraID).getTexture();
}
//--------------------------------------------------------------
float ofxEWCLIB::getSampleTime(int cameraID) {
	return (cameraID < 0) ? ewc_s[lastCounts.begin()->first].stime : ewc_s[cameraID].stime;
}
float ofxEWCLIB::getFrameTime(int cameraID) {
	return (cameraID < 0) ? ewc_s[lastCounts.begin()->first].ftime : ewc_s[cameraID].ftime;
}
float ofxEWCLIB::getFps(int cameraID) {
	return 1.0 / ((cameraID < 0) ? ewc_s[lastCounts.begin()->first].ftime : ewc_s[cameraID].ftime);
}
int ofxEWCLIB::getCount(int cameraID) {
	return (cameraID < 0) ? ewc_s[lastCounts.begin()->first].count : ewc_s[cameraID].count;
}
//--------------------------------------------------------------
int ofxEWCLIB::getCameraNum() {
	return EWC_GetCamera();
}
//--------------------------------------------------------------
string ofxEWCLIB::getDeviceName(int cameraID) {
	return EWC_GetDeviceName(cameraID);
}
//--------------------------------------------------------------
bool ofxEWCLIB::openProperty(int cameraID) {
	return (cameraID < 0) ? EWC_PropertyPage(lastCounts.begin()->first) : EWC_PropertyPage(cameraID);
}
