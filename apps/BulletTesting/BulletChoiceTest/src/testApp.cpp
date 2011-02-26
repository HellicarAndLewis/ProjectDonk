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
	//bullet.createGround(333);
	bullet.world->setGravity(btVector3(0, 0, 0));
	
	ofVec3f pos(-80, 0, 0);
	momA = bullet.createSphere(pos, 50, 100);
	
	ofVec3f posb(80, 0, 0);
	momB = bullet.createSphere(posb, 50, 100);
	
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(btScalar(10.), btScalar(0.), btScalar(0.)));
	
	ofxBulletRigidBody* anchor = bullet.createSphere(pos, 10, 0);
	sA = makeSpring(anchor->body,momA->body);
	
	tr.setIdentity();
	tr.setOrigin(btVector3(btScalar(10.), btScalar(0.), btScalar(0.)));
	
	ofxBulletRigidBody* anchorB = bullet.createSphere(posb, 10, 0);
	sB = makeSpring(anchorB->body,momB->body);
	
}

//--------------------------------------------------------------
void testApp::exit() {
	bullet.destroy();
}

//--------------------------------------------------------------
void testApp::update() {
	
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
	
	glTranslatef(ofGetWidth()/2, (ofGetHeight()/2)+0, 400);
	glScalef(SCALE, SCALE, SCALE);
	//ofRotateX(ofGetMouseY());
	//ofRotateY(ofGetMouseX());
	
	bullet.draw();
	
	ofSetColor(255, 255, 255);
	//bullet.drawFloor();
	
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor4f(0, 0, 0, 1);
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
		
		ofxBulletRigidBody * b;
		b = bullet.createSphere(pos, ofRandom(3, 20), 1);
		if( int(ofRandom(0,2))%2 == 0 ) bullet.addAttractor(b,momA,40);
		else bullet.addAttractor(b,momB,40);

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
			if( bullet.rigidBodies[i] == momA || bullet.rigidBodies[i] == momB) continue;
			
			if( int(ofRandom(0,2))%2 == 0 ) bullet.addAttractor(bullet.rigidBodies[i],momA,20);
			else bullet.addAttractor(bullet.rigidBodies[i],momB,20);
		}
	}
	
	if( key == 'q'){
		
		//bullet.clearConstraints();
		//bullet.world->removeConstraint(sA);
		//bullet.world->removeConstraint(sB);
		//momA->setSize(1);
		momA->destroy();
		momB->destroy();
		
	}
	
	if(key =='k')
	{
	momA->setSize(60);
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

btGeneric6DofSpringConstraint* testApp::makeSpring(btRigidBody* pBodyA,btRigidBody* pBodyB)
{

	btTransform frameInA, frameInB;
	frameInA = btTransform::getIdentity();
	frameInA.setOrigin(btVector3(btScalar(1.), btScalar(0.), btScalar(0.)));
	frameInB = btTransform::getIdentity();
	frameInB.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));
	
	btGeneric6DofSpringConstraint* pGen6DOFSpring = new btGeneric6DofSpringConstraint(*pBodyA, *pBodyB, frameInA, frameInB, true);
	pGen6DOFSpring->setLinearUpperLimit(btVector3(100., 100., 100.));
	pGen6DOFSpring->setLinearLowerLimit(btVector3(-100., -100., -100.));
	
	pGen6DOFSpring->setAngularLowerLimit(btVector3(0.f, 0.f, 0));
	pGen6DOFSpring->setAngularUpperLimit(btVector3(0.f, 0.f, 0));
	
	bullet.world->addConstraint(pGen6DOFSpring, true);
	pGen6DOFSpring->setDbgDrawSize(btScalar(5.f));
	
	for(int i = 0; i < 6; i++)
	{
		pGen6DOFSpring->enableSpring(i,true);
		pGen6DOFSpring->setStiffness(i, 40);
		pGen6DOFSpring->setDamping(i, .0015);
	}
	
	return pGen6DOFSpring;

}

