#include "testApp.h"
#include "BubbleData.h"
#include "QuestionData.h"
#include "constants.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	setupOsc();
	setupKinect();
	setupGraphics();
	mode = Donk::Mode::getInstance();
	// default starting mode
	nextMode = "buzz";
	mode->setMode(nextMode);

}

void testApp::setupGraphics() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60.f);
	ofBackground(0, 0, 0);

	projection.allocate(PROJECTION_RESOLUTION_WIDTH, PROJECTION_RESOLUTION_HEIGHT);
	calibrationProjection.allocate(PROJECTION_RESOLUTION_WIDTH, PROJECTION_RESOLUTION_HEIGHT);
	
}
//--------------------------------------------------------------
void testApp::update(){

	//call update on all the bubbles
	std::vector<Donk::BubbleData*>::iterator bdit;
	for(bdit=bubbles.begin();bdit!=bubbles.end();bdit++){
		(*bdit)->step();
	}
	
	processOsc();
	projection.update();
}


void testApp::render() {
	
	//draw all the bubbles
	glPushMatrix();
	int xoff = 0;
	int yoff = 0;
	std::vector<Donk::BubbleData*>::iterator bdit;
	for(bdit=bubbles.begin();bdit!=bubbles.end();bdit++){
		if((*bdit)->profileImage.width != 0){
			
			if(xoff+(*bdit)->profileImage.width>ofGetWidth()){
				yoff+=50;
				xoff = 0;
			}
			
			ofSetColor(255,255,255);
			(*bdit)->profileImage.draw(xoff,yoff);
		
			xoff += (*bdit)->profileImage.width;
			
		}
	}
	glPopMatrix();
		
	
	
	
	
	// if we're capturing coords for projection mapping...
	if(calibrationProjection.calibrate) {
		calibrationProjection.render();
	} else {
		projection.render();
	}
}

void testApp::drawView() {
	
	// comment this stuff out if you don't want to draw the mesh
	/*ofSetColor(mode->getValue("Background Red"), 
			  mode->getValue("Background Green"), 
			  mode->getValue("Background Blue"));
	*/
	
	if(calibrationProjection.calibrate) {
		
		calibrationProjection.drawOnModel(scene->getModel());
		if(calibrationProjection.drawFacets) {
			glLineWidth(calibrationProjection.lineWidth);
			ofSetHexColor(calibrationProjection.facetColor);
			scene->getModel()->drawOutline();
			glLineWidth(1);
		}
	} else {
		projection.drawOnModel(scene->getModel());
	}
	
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
	ofTouchEventArgs t;
	t.x = (float)x/ofGetWidth();
	t.y = (float)y/ofGetHeight();
	t.id = button;
	touchMoved(t);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	ofTouchEventArgs t;
	t.x = (float)x/ofGetWidth();
	t.y = (float)y/ofGetHeight();
	t.id = button;
	touchDown(t);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	ofTouchEventArgs t;
	t.x = (float)x/ofGetWidth();
	t.y = (float)y/ofGetHeight();
	t.id = button;
	touchUp(t);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------

void testApp::setupOsc() {
	if(!json_settings.loadFromFile("settings/OSC.json.txt")){
		cerr << "failed to load json settings file" << endl;
		_exit(1);
	}
	oscIn.setup(json_settings["osc_listen_port"].asInt());
	
}

void testApp::processOsc() {
	while(oscIn.hasWaitingMessages()){
		ofxOscMessage m;
		oscIn.getNextMessage(&m);
		if(m.getAddress()=="/control/mode/next"){
			
			//get the next mode ready
			string modeName = m.getArgAsString(0);
			nextMode = modeName;
			
			//			cout << "todo: start upcoming mode standby for mode: " << modeName << endl;
			
		}else if(m.getAddress()=="/control/mode/swap"){
			
			mode->setMode(nextMode);
			//			cout << "todo: begin transition from current to upcoming mode." << endl;
			
		}else if(m.getAddress()=="/control/bubble/new"){
			
			bubbles.push_back(new Donk::BubbleData(m));
			//cout << "todo: do something with new bubble received from " << bd.userName << " in mode " << bd.mode << endl;
			
		}else if(m.getAddress()=="/control/question/update"){
			
			Donk::QuestionData qd(m);
			cout << "todo: handle question update \"" << qd.text << "\" " << qd.tags[0] << "=" <<
			qd.tag_counts[0] << "," << qd.tags[1] << "=" << qd.tag_counts[1] << endl;
			
		}else if(m.getAddress()=="/audio"){
			
			//get 6 floated values
			float audioData[6];
			for(int i=0;i<6;i++)audioData[i] = m.getArgAsFloat(i);
			
			cout << "todo: do something with new audio frequency sample: " <<
				audioData[0] << ' ' <<
				audioData[1] << ' ' <<
				audioData[2] << ' ' <<
				audioData[3] << ' ' <<
				audioData[4] << ' ' <<
				audioData[5] << endl;
		}
	}
}

void testApp::setupKinect() {
	tuio.connect(json_settings["tuio_listen_port"].asInt());
	ofAddListener(ofEvents.touchDown, this, &testApp::touchDown);
	ofAddListener(ofEvents.touchUp, this, &testApp::touchUp);
	ofAddListener(ofEvents.touchMoved, this, &testApp::touchMoved);

	calibrationProjection.setInteractiveArea(&projection.getInteractiveArea());
	calibrationProjection.createGui(getCalibrationGui());
	
	
}

void testApp::touchDown(ofTouchEventArgs &touch) {
	projection.touchDown(touch.x, touch.y, touch.id);
}

void testApp::touchMoved(ofTouchEventArgs &touch) {
	projection.touchMoved(touch.x, touch.y, touch.id);
	// e.g.
	// touches are normalized
	ofVec2f pos(touch.x, touch.y);
	
	// now scaled for screen coords
	pos *= ofGetWindowSize();
	//printf("pos: %f %f\n", pos.x, pos.y);
}

void testApp::touchUp(ofTouchEventArgs &touch) {
	projection.touchUp(touch.x, touch.y, touch.id);
}