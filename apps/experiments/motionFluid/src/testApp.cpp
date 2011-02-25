#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//ofEnableAlphaBlending();
	
	/*cam.resetTransform();
	cam.setPosition(40, 40, 100);
	cam.setFov(60);*/
	
	//emitterPoint.set(100, 100, 100);
	
	ofSetSphereResolution(16);
	ofBackground(0);
	ofEnableLighting();
	glEnable(GL_DEPTH_TEST);
	
	ofSetFrameRate(60);
	
	light.setDirectional(false);
	light.setAmbientColor(ofColor(150, 150, 150));
	light.setDiffuseColor(ofColor(255, 255, 255));
	
	drawSphere = false;
	
	emitter.set(500, ofGetHeight(), 0);
}

//--------------------------------------------------------------
void testApp::update(){
	
	vector<ofVec3f>::iterator dObj = disturbingObjects.begin();
	
	while(dObj != disturbingObjects.end())
	{
		if( dObj->y > 0 ) {
			dObj->y -= 7.f;
			if( ofRandomuf() < .4f) {
				float scale = min(ofGetHeight() / dObj->y, 10.f);
				particles.push_back( new BrownianObject( *dObj, scale + 2));
			}
			++dObj;
		} else {
			dObj = disturbingObjects.erase(dObj);
		}
	}
	
	
	if(ofRandomuf() < 0.1)
	{
		
		particles.push_back( new BrownianObject( emitter, 4));
	}
	
	vector<BrownianObject*>::iterator it = particles.begin();
	while(it != particles.end())
	{
		if( (*it)->isDead()) {
			if(*it) {
				delete *it;
			}
			it = particles.erase(it);
		} else {
			it++;
		}
	}
	

}

//--------------------------------------------------------------
void testApp::draw(){
		
	light.setPosition(ofVec3f(500, 20, 0));
	light.enable();
	light.draw();

	ofClear(0, 0, 0, 255);
	//ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
	
	vector<ofVec3f>::iterator dObj = disturbingObjects.begin();
	while(dObj != disturbingObjects.end())
	{
		ofSphere(dObj->x, dObj->y, dObj->z, 15);
		++dObj;
	}
	
	vector<BrownianObject*>::reverse_iterator it = particles.rbegin();
	while(it != particles.rend())
	{
		(*it)->draw();
		it++;
	}
	
}

void testApp::createObjectAtPoint(ofVec3f point)
{
	
	disturbingObjects.push_back(point);
	
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
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	//drawSphere = true;
	
	ofVec3f vec(x, y, ofRandom(-10));
	createObjectAtPoint(vec);
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	//drawSphere = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

