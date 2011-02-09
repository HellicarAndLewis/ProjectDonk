#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	tuioHost = "localhost";
	tuioPort = 3333;
	planarKinect.set(20, 20, 320, 240);
	planarKinect.setup();
	kinect.init();
	kinect.open(planarKinect.deviceId);
	ofSetFrameRate(30);
	ofBackground(0,0,0);
	blobTracker.addListener(this);
	tuioServer.start((char*)tuioHost.c_str(), tuioPort);
	tuioServer.setVerbose(true);
	
	
	setupGui();
}

void testApp::setupGui() {
	
	// this sets up and lays out the gui.
	
	gui.setup(0, 0);
	gui.size(ofGetWidth(), ofGetHeight());
	gui.setLayoutType(LAYOUT_ABSOLUTE);
	GuiControl *k = gui.addDrawable("kinect", planarKinect);
	k->set(10, 10, 320, 240);
	
	k = gui.addSegmentedControl("Mode", planarKinect.guiMode, "Draw Threshold|Select Slice")->under(k, 10)->size(320,20);	
	
	k = gui.addTitle("Corner calibration")->under(k);
	// put 1 under kinect
	GuiControl *c = gui.addButton("1")->under(k)->size(20, 20);
	
	// put 4 under 1
	c =	gui.addButton("4")->under(c)->size(20, 20);

	
	
	// put 2 and 3 under and to the right of kinect
	c = gui.addButton("2")->size(20, 20)->right(gui.getControlById("1"));
	gui.addButton("3")->size(20, 20)->under(c);
	
	gui.addListener((GuiListener*)this);
	gui.enable();
	
}


//--------------------------------------------------------------
void testApp::update(){
	
	kinect.update();

	planarKinect.update(kinect.getDepthPixels());
	blobTracker.track(planarKinect.blobs);
	tuioServer.run();
}

//--------------------------------------------------------------
void testApp::draw(){
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if(key=='h') {
		system((string("open ")+ofToDataPath("instructions.gif", true)).c_str());
	}
	if(key=='1') {
		planarKinect.calibrateCorner(TOP_LEFT_CORNER);
	} else if(key=='2') {
		planarKinect.calibrateCorner(TOP_RIGHT_CORNER);
	} else if(key=='3') {
		planarKinect.calibrateCorner(BOTTOM_RIGHT_CORNER);
	} else if(key=='4') {
		planarKinect.calibrateCorner(BOTTOM_LEFT_CORNER);
	}
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	planarKinect.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	planarKinect.mousePressed(x, y, button);
	

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	planarKinect.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

void testApp::blobEntered(ofVec3f pos, int blobId) {
	
	// ofxTuioServer assumes you're giving screen coordinates, so multiply by ofGetWidth()/Height()
	blobs[blobId] = tuioServer.addCursor(pos.x*ofGetWidth(), pos.y*ofGetHeight());
}
void testApp::blobMoved(ofVec3f pos, int blobId) {
	tuioServer.updateCursor(blobs[blobId], pos.x*ofGetWidth(), pos.y*ofGetHeight());
}

void testApp::blobExited(ofVec3f pos, int blobId) {
	tuioServer.removeCursor(blobs[blobId]);
	blobs.erase(blobId);
}

void testApp::controlChanged(GuiControl *control) {
	if(control->controlId=="1") {
		planarKinect.calibrateCorner(TOP_LEFT_CORNER);
	} else if(control->controlId=="2") {
		planarKinect.calibrateCorner(TOP_RIGHT_CORNER);
	} else if(control->controlId=="3") {
		planarKinect.calibrateCorner(BOTTOM_RIGHT_CORNER);
	} else if(control->controlId=="4") {
		planarKinect.calibrateCorner(BOTTOM_LEFT_CORNER);
	}
}