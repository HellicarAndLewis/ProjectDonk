#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	bApplyForce = false;
	ofSetSphereResolution(4);
	ofBackground(255,255,255);
	ofEnableAlphaBlending();
	ofEnableLighting();
	glEnable(GL_DEPTH_TEST);
	
	light.setDirectional(true);
	light.setAmbientColor(ofColor(100, 120, 100));
	light.setDiffuseColor(ofColor(255, 255, 255));
	
	bullet.init();
	//bullet.createGround(333);
	bullet.world->setGravity(btVector3(0, -10, 0));
	
	int w = 800;
	int h = 800;
	int d = 100;
	
	ofVec3f pos(0, 0, 0);
	momA = bullet.createSphere(pos, 1, 0);
	prevTime = ofGetElapsedTimef();
	cout << bullet.rigidBodies.size()-1<< endl;
	
	wallA = bullet.createBox(ofVec3f(0,0,-d), ofVec3f(w,h,1), 0);
	wallB = bullet.createBox(ofVec3f(0,0,d), ofVec3f(w,h,1), 0);
	wallC = bullet.createBox(ofVec3f(0,h,0), ofVec3f(w,1,d), 0);
	//bullet.createBox(ofVec3f(w,0,0), ofVec3f(1,h,d), 0);
	//bullet.createBox(ofVec3f(-w,0,0), ofVec3f(1,h,d), 0);
	bullet.createBox(ofVec3f(0,-h,0), ofVec3f(w,1,d), 0);
	
		
	
}

//--------------------------------------------------------------
void testApp::exit() {
	bullet.destroy();
}

//--------------------------------------------------------------
void testApp::update() {
	
	float dt = ofGetElapsedTimef()-prevTime;
	
	
	for(int i=0; i<bullet.rigidBodies.size(); i++)
	{
	ofVec3f pos =  bullet.rigidBodies[i]->getPosition();
	if(pos.x > 1000 || pos.x < -1000) bullet.rigidBodies[i]->destroy();
	}
	
	
	bullet.update();
	
	for(int i=0; i<bullet.rigidBodies.size(); i++){
		if(
		   bullet.rigidBodies[i]->shape->getShapeType() != BOX_SHAPE_PROXYTYPE && 
		   bullet.rigidBodies[i] != momA) 
		{
			float radius = bullet.rigidBodies[i]->radius;
			if(radius < 100 ) radius+=100*dt;
			bullet.rigidBodies[i]->setSize(radius);
		}
	    
		
	}
	
	//cout << "rand " <<  ofRandom(8,bullet.rigidBodies.size()-1) << endl;
	//cout << "bodies: " << bullet.rigidBodies.size() << endl;
	//cout << "consts: "  << bullet.constraints.size() << endl;
	
	prevTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void testApp::draw() {
	
	light.setPosition(ofVec3f(0, 20, 0));
	light.enable();
	light.draw();
	glEnable(GL_DEPTH_TEST);
	
	glPushMatrix();
	
	glTranslatef(ofGetWidth()/2, (ofGetHeight()/2)+0, -400);
	glScalef(SCALE, SCALE, SCALE);
	//ofRotateX(ofGetMouseY());
	//ofRotateY(ofGetMouseX());
	
	//bullet.draw();
	
	for	(int i=0; i<bullet.rigidBodies.size(); i++) {
		ofSetColor(200, 200, 205,100);
		if(bullet.rigidBodies[i]->shape->getShapeType() != BOX_SHAPE_PROXYTYPE)
			bullet.rigidBodies[i]->draw();
		
	}
	
	ofSetColor(255, 255, 255);
	//bullet.drawFloor();
	
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor4f(0, 0, 0, 1);
	ofDrawBitmapString( "space: add random sphere | x: remove random sphere", 20, 15);

	ofSetColor(255, 0, 0);
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 'f') {
		ofToggleFullscreen();	
	}
	
	if(key == ' ') {
		
		ofVec3f pos(ofRandom(-250, 250), ofRandom(-250, 250), 0);
		
		ofxBulletRigidBody * b = bullet.createSphere(pos, 10, 1);
		b->body->setDamping(.2, .9);
		bullet.addAttractor(b,momA,30);
	}
	
	if( key == 'x')
	{
		int randBull = ofRandom(0,bullet.rigidBodies.size()-1);
		if(
		   bullet.rigidBodies[randBull]->shape->getShapeType() != BOX_SHAPE_PROXYTYPE && 
		   bullet.rigidBodies[randBull] != momA) 
			bullet.rigidBodies[randBull]->bDestroy = true;
			//cout << "rand remove " << randBull << endl;
		
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

