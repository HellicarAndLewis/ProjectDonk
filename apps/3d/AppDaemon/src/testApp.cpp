#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	webServer.start();
	webServer.addHandler(this, "*");
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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

void testApp::httpGet(string url) {
	char data[1024];
	FILE *fp = popen("ps aux | grep Kinect", "r");
	if (fp == NULL) {
		httpResponse("Error getting data\n");
	} else {		
		string s = "";
		while (fgets(data, 1024, fp) != NULL) {
			s += string(data);
		}
		
		//system("/ProjectDonk/apps/3d/KinectTracker/bin/KinectTracker.app/Contents/MacOS/KinectTracker");
		httpResponse(s);
		pclose(fp);
	}
}