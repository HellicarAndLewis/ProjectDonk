#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	bApplyForce = false;
	ofSetSphereResolution(4);
	ofBackground(255,255,255);
	ofEnableLighting();
	glEnable(GL_DEPTH_TEST);
	
	light.setDirectional(true);
	light.setAmbientColor(ofColor(100, 120, 100));
	light.setDiffuseColor(ofColor(255, 255, 255));
	
	bullet.init();
	bullet.createGround(333);
	bullet.world->setGravity(btVector3(0, 0, 0));
	
	ofVec3f posa(ofRandom(-50, 50), ofRandom(0, 150), ofRandom(-50, 50));
	ofxBulletRigidBody * a = bullet.createSphere(posa, ofRandom(3, 20), 1);
	
	ofVec3f posb(ofRandom(-50, 50), 150, ofRandom(-50, 50));
	mom = bullet.createSphere(posb, 100, 0);
	ofVec3f posb2(posb.x+200, 150, ofRandom(-50, 50));
	mom2 = bullet.createSphere(posb2, 70, 0);
	bullet.addAttractor(a,mom2,40);
	
}

//--------------------------------------------------------------
void testApp::exit() {
	bullet.destroy();
}

//--------------------------------------------------------------
void testApp::update() {
	
	for	(int i=0; i<bullet.rigidBodies.size(); i++) {
		
		// remove shapes that are out of bounds
		ofVec3f p = bullet.rigidBodies[i]->getPosition();
		if(p.y < -3) {
			bullet.rigidBodies[i]->bDestroy = true;
		}
		
	}
	
	
	bullet.update();
	cout << bullet.rigidBodies.size() << endl;
}

//--------------------------------------------------------------
void testApp::draw() {
	
	light.setPosition(ofVec3f(0, 20, 0));
	light.enable();
	light.draw();
	glEnable(GL_DEPTH_TEST);
	
	glPushMatrix();
	
	glTranslatef(ofGetWidth()/2, (ofGetHeight()/2), 40);
	glScalef(SCALE, SCALE, SCALE);
	ofRotateX(90);//ofGetMouseY());
	//ofRotateY(90);//ofGetMouseX());
	
	bullet.draw();
	
	ofSetColor(255, 255, 255);
	bullet.drawFloor();
	
	glPopMatrix();
	
	
	
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor4f(1, 1, 1, 1);
	ofDrawBitmapString( "space: add random sphere | x: clear attractions  | z: add attractions", 20, 15);

	ofSetColor(255, 0, 0);
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 'f') {
		ofToggleFullscreen();	
	}
	
	if(key == ' ') {
		
		ofVec3f pos(ofRandom(-50, 50), ofRandom(0, 150), ofRandom(-50, 50));
		
		ofxBulletRigidBody * a;
		a = bullet.createSphere(pos, ofRandom(3, 20), 1);
		bullet.addAttractor(a,mom,100);
		bullet.addAttractor(a,mom2,40);

	}
	
	if( key == 'x')
	{
		bullet.clearConstraints();
		//for	(int i=0; i<bullet.rigidBodies.size(); i++)
		//	bullet.clearAttractions(bullet.rigidBodies[i]);
	}
	
	if( key == 'z')
	{
		for	(int i=0; i<bullet.rigidBodies.size(); i++)
		{	
		bullet.addAttractor(bullet.rigidBodies[i],mom,10);
		bullet.addAttractor(bullet.rigidBodies[i],mom2,5);
		}
	}
	
	if( key == 'q')	bullet.clearConstraints();
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
