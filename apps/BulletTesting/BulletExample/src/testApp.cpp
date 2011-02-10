#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	bApplyForce = false;
	ofSetSphereResolution(4);
	ofBackground(0);
	ofEnableLighting();
	glEnable(GL_DEPTH_TEST);
	
	light.setDirectional(false);
	light.setAmbientColor(ofColor(100, 120, 100));
	light.setDiffuseColor(ofColor(255, 255, 255));
	
	
	bullet.init();
	bullet.createGround(333);
	bullet.world->setGravity(btVector3(0, -10, 0));
	
}

//--------------------------------------------------------------
void testApp::exit() {
	bullet.destroy();
}

//--------------------------------------------------------------
void testApp::update() {
	
	for	(int i=0; i<bullet.rigidBodies.size(); i++) {
		if(bApplyForce) {
			btRigidBody * b = bullet.rigidBodies[i]->body;
			b->clearForces();
			ofVec3f c = ofVec3f(0, 10, 0);
			ofVec3f p = bullet.rigidBodies[i]->getPosition();
			ofVec3f v = c-p;
			v.normalize();
			v *= 105.0;
			
			b->applyCentralForce(btVector3(v.x, v.y, v.z));
			
		}
		
		
		// remove shapes that are out of bounds
		ofVec3f p = bullet.rigidBodies[i]->getPosition();
		if(p.y < -3) {
			bullet.rigidBodies[i]->bDestroy = true;
		}
		
		
	}
	
	
	bullet.update();
}

//--------------------------------------------------------------
void testApp::draw() {
	
	light.setPosition(ofVec3f(0, 20, 0));
	light.enable();
	light.draw();
	
	
	glPushMatrix();
	//glTranslatef(ofGetWidth()/2, (ofGetHeight()/2), 0);
	glTranslatef(ofGetWidth()/2, (ofGetHeight()/2)+100, 40);
	glScalef(SCALE, SCALE, SCALE);
	ofRotateX(ofGetMouseY());
	ofRotateY(ofGetMouseX());
	
	

	//ofDrawAxis(200);
	
	bullet.draw();
	
	ofSetColor(255, 255, 255);
	bullet.drawFloor();
	
	glPopMatrix();
	
	ofSetColor(255, 0, 0);
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 'f') {
		ofToggleFullscreen();	
	}
	if(key == 'm') {
		bApplyForce = !bApplyForce;
	}
	
	if(key == ' ') {
		
		ofVec3f pos(ofRandom(-50, 50), ofRandom(0, 150), ofRandom(-50, 50));

		if((int)ofRandom(2) == 0) {
			bullet.createSphere(pos, ofRandom(3, 20), 1);
		}
		else {
			ofVec3f size(10, 10, 10);
			bullet.createBox(pos, size, 1);
		}
		
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
