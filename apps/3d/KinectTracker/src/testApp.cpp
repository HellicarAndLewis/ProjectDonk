#include "testApp.h"
#include "ofxSettings.h"
//--------------------------------------------------------------
void testApp::setup(){
	tuioHost = "localhost";
	tuioPort = 3333;
	ofSetFrameRate(30);
	ofBackground(0,0,0);

	
	planarKinect.setup();
	kinect.init();
	kinect.open(planarKinect.deviceId);

	blobTracker.addListener(this);
	setupGui();
	
	tuioServer.start((char*)tuioHost.c_str(), tuioPort);
	tuioServer.setVerbose(true);
	
	
	
}

void testApp::setupGui() {
	
	// this sets up and lays out the gui.
	
	gui.setup(0, 0);
	gui.size(ofGetWidth(), ofGetHeight());
	gui.setLayoutType(LAYOUT_ABSOLUTE);
	GuiControl *k = gui.addTitle("Kinect Input");
	k->set(10, 10, 320, 20);
	k = gui.addDrawable("kinect", planarKinect)->under(k)->size(320, 240);
	
	k = gui.addSegmentedControl("Mode", planarKinect.guiMode, "Draw Threshold|Select Slice")->under(k, 10)->size(320,20);	
	
	
	// put 1   2
	//     4   3
	// buttons to calibrate corners.
	k = gui.addTitle("Corner calibration")->under(k);
	
	// put 1 under kinect
	GuiControl *c = gui.addButton("1")->under(k)->size(20, 20);
	
	// put 4 under 1
	c =	gui.addButton("4")->under(c)->size(20, 20);
	
	
	// put 2 and 3 under and to the right of kinect
	c = gui.addButton("2")->size(20, 20)->right(gui.getControlById("1"));
	gui.addButton("3")->size(20, 20)->under(c);
	
	
	// do threshold controls.
	c = gui.addTitle("Threshold Controls")->underRight(gui.getControlById("Mode"));
	c = gui.addButton("Capture Background")->under(c);
	gui.addButton("down")->size(30, 20)->under(c);
	gui.addButton("up")->size(30, 20)->underRight(c);

	c = gui.addTitle("TUIO Output")->size(320, 20)->under(gui.getControlById("4"));
	c = gui.addDrawable("blobs", blobTracker)->under(c)->size(320, 240);
	c = gui.addSlider("smoothness", blobTracker.smoothing, 0, 0.99)->under(c, 10);
	c = gui.addTextField("TUIO Host", tuioHost)->under(c, 10)->size(70, 20);
	c = gui.addIntField("Port", tuioPort)->right(c)->size(40, 20);
	c = gui.addButton("Reconnect")->right(c)->size(70, 20);
	c = gui.addButton("Help")->right(c, 69)->size(60, 20);
	gui.enableAutoSave("trackerSettings.xml");
	gui.addListener((GuiListener*)this);
	
	gui.enable();
	
}


//--------------------------------------------------------------
void testApp::update(){
	
	kinect.update();

	planarKinect.update(kinect.getDepthPixels());
	 blobTracker.track(planarKinect.blobs);
	tuioServer.run();
	
	ofSetWindowTitle("KinectTracker - "+ofToString(ofGetWidth()) + "x"+ofToString(ofGetHeight()));
}

//--------------------------------------------------------------
void testApp::draw(){
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
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
	} else if(control->controlId=="up") {
		planarKinect.moveThreshold(1);
	} else if(control->controlId=="down") {
		planarKinect.moveThreshold(-1);
	} else if(control->controlId=="Capture Background") {
		planarKinect.captureThreshold();
	} else if(control->controlId=="Reconnect") {
		tuioServer.start((char*)tuioHost.c_str(), tuioPort);
	} else if(control->controlId=="Help") {
		system((string("open ")+ofToDataPath("instructions.pdf", true)).c_str());
	}
}
