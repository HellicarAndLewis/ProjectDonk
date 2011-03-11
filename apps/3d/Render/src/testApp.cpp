#include "testApp.h"
#include "BubbleData.h"
#include "QuestionData.h"
#include "constants.h"

// for testing
bool bDidFakeSecondTouch = false;

testApp *testApp::instance;
//--------------------------------------------------------------
void testApp::setup(){
	projection = BubbleProjection::getInstance();
	audioData = Donk::AudioData::getInstance();
	
	gradientProjection.allocate(500, 500);
	instance = this;
	
	setupOsc();
	setupKinect();
	setupGraphics();
	mode = Donk::Mode::getInstance();
	// default starting mode
	nextMode = "inspiration";
	mode->setMode(nextMode);
}

void testApp::setupGraphics() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60.f);
	ofBackground(0, 0, 0);

	projection->allocate(PROJECTION_RESOLUTION_WIDTH, PROJECTION_RESOLUTION_HEIGHT);
	calibrationProjection.allocate(PROJECTION_RESOLUTION_WIDTH, PROJECTION_RESOLUTION_HEIGHT);
	
}
//--------------------------------------------------------------
void testApp::update(){

	Donk::BubbleData::update();
	
	processOsc();
	projection->update();
	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 2) + "fps,  " + ofToString(ofGetWindowSize().x) + "x"+ofToString(ofGetWindowSize().y));
	
	
	
}


void testApp::render() {
	
	// if we're capturing coords for projection mapping...
	if(calibrationProjection.calibrate) {
		calibrationProjection.render();
	} else {
		projection->render();
		gradientProjection.render();
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
		
		
		
		
		scene->getLight()->setPosition(ofVec3f(mode->getValue("light x"), 
								  mode->getValue("light y"), 
								  mode->getValue("light z")));
		glShadeModel(GL_FLAT);
		ofEnableLighting();
		scene->getLight()->enable();
//		scene->getModel()->drawSolid();
		gradientProjection.drawOnModel(scene->getModel());
		
		
		glShadeModel(GL_SMOOTH);
		ofDisableLighting();
		projection->drawOnModel(scene->getModel());
	}
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key == 'q') mode->setMode("buzz");
	if(key == 'w') mode->setMode("inspiration");
	if(key == 'e') mode->setMode("interview");
	if(key == 'r') mode->setMode("question");
	if(key == 't') mode->setMode("performance");
	
	
	
	if(key=='b'||key=='B') {
		ofxOscMessage m;
		Donk::BubbleData *data = new Donk::BubbleData(m);
		data->mode = "buzz";
		data->id = "39A5D49FE5";
		switch((int)ofRandom(5)) {
			case 0: data->text = "This is test 1"; break;
			case 1: data->text = "Bubble bubble bubble"; break;
			case 2: data->text = "Project donk here"; break;
			case 3: data->text = "Who is this Maroon 5?"; break;
			case 4: data->text = "brown sticky liquid!"; break;
		}
		switch((int)ofRandom(5)) {
			case 0: data->userName = "mazbox"; break;
			case 1: data->userName = "cokeMe!"; break;
			case 2: data->userName = "DeadSaxon"; break;
			case 3: data->userName = "jtnimoy"; break;
			case 4: data->userName = "timeteam"; break;
		}
		
		
		if(ofRandomuf()>0.5){
			data->media.push_back(Donk::BubbleData::MediaEntry());
			char fname[64];
			sprintf(fname,"images/performance/%i.png",(int)ofRandom(1,13));
			cout << fname << endl;
			data->media.back().mediaImage.loadImage(fname);
			data->media.back().thumbImage = data->media.back().mediaImage;
			data->media.back().thumbImage.resize(128, 128);
		}
		
		data->profileImage.loadImage("lena.png");

		
		projection->bubbleReceived(data);
	}

	
	
	if(!bDidFakeSecondTouch && key == 'x') {
		ofTouchEventArgs t;
		t.id = 1;
		t.x = (float)mouseX/ofGetWidth();
		t.y = (float)mouseY/ofGetHeight();
 		touchDown(t);
		bDidFakeSecondTouch = true;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

	if(bDidFakeSecondTouch) {
		ofTouchEventArgs t;
		t.id = 1;
		t.x = (float)mouseX/ofGetWidth();
		t.y = (float)mouseY/ofGetHeight();
 		touchUp(t);
		bDidFakeSecondTouch = false;
	}
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
	
	if(ofGetKeyPressed('x')) {
		
		t.id ++;
		float pct = 100.0 * ((float)x/(float)ofGetWidth());
		t.x = (float)(x-pct)/ofGetWidth();
		t.y = (float)y/ofGetHeight();
		if(!bDidFakeSecondTouch) {
			touchDown(t);
			bDidFakeSecondTouch = true;
		}
		else touchMoved(t);
		
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	ofTouchEventArgs t;
	t.x = (float)x/ofGetWidth();
	t.y = (float)y/ofGetHeight();
	t.id = button;
	touchDown(t);
	
	if(ofGetKeyPressed('x')) {
		t.id = 1;
		t.x = (float)(x-50.0)/ofGetWidth();
		t.y = (float)y/ofGetHeight();
 		touchDown(t);
		bDidFakeSecondTouch = true;
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	ofTouchEventArgs t;
	t.x = (float)x/ofGetWidth();
	t.y = (float)y/ofGetHeight();
	t.id = button;
	touchUp(t);
	
	if(bDidFakeSecondTouch) {
		t.id = 1;
		t.x = (float)(x-50.0)/ofGetWidth();
		t.y = (float)y/ofGetHeight();
 		touchUp(t);
		bDidFakeSecondTouch = false;
	}
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
	audioData->connect(json_settings["audio_osc_port"].asInt());
	
}

void testApp::processOsc() {
	while(oscIn.hasWaitingMessages()) {
		ofxOscMessage m;
		oscIn.getNextMessage(&m);
		if(m.getAddress()=="/control/mode/next") {
			
			//get the next mode ready
			string modeName = m.getArgAsString(0);
			nextMode = modeName;
						
		}else if(m.getAddress()=="/control/mode/swap") {			
			mode->setMode(nextMode);
			
		}else if(m.getAddress()=="/control/bubble/new") {
			
			Donk::BubbleData::add(m);	
			
		}else if(m.getAddress()=="/control/question/update") {
			
			Donk::QuestionData qd(m);
			cout << "todo: handle question update \"" << qd.text << "\" " << qd.tags[0] << "=" <<
			qd.tag_counts[0] << "," << qd.tags[1] << "=" << qd.tag_counts[1] << endl;
			
		}
	}
	audioData->update();
}

void testApp::setupKinect() {
	tuio.connect(json_settings["tuio_listen_port"].asInt());
	ofAddListener(ofEvents.touchDown, this, &testApp::touchDown);
	ofAddListener(ofEvents.touchUp, this, &testApp::touchUp);
	ofAddListener(ofEvents.touchMoved, this, &testApp::touchMoved);

	calibrationProjection.setInteractiveArea(&projection->getInteractiveArea());
	calibrationProjection.createGui(getCalibrationGui());
	
	
}

void testApp::touchDown(ofTouchEventArgs &touch) {
	projection->touchDown(touch.x, touch.y, touch.id);
}

void testApp::touchMoved(ofTouchEventArgs &touch) {
	projection->touchMoved(touch.x, touch.y, touch.id);
	// e.g.
	// touches are normalized
	ofVec2f pos(touch.x, touch.y);
	
	// now scaled for screen coords
	pos *= ofGetWindowSize();
	//printf("pos: %f %f\n", pos.x, pos.y);
}

void testApp::touchUp(ofTouchEventArgs &touch) {
	projection->touchUp(touch.x, touch.y, touch.id);
}