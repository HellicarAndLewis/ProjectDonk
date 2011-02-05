#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	initBullet();
	ofVec3f origin(500, 500, 0);
	ofVec2f size(300, 300);
	containing = createContainingBubble(origin, size);
	origin.x += 100;
	origin.y += 100;
	createContainedBubble(origin, 20, containing);
}

//--------------------------------------------------------------
void testApp::update(){
	
	updateBullet();

}

//--------------------------------------------------------------
void testApp::draw(){

	drawBullet();
	
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

void testApp::drawBullet()
{
	int numObjects = m_dynamicsWorld->getNumCollisionObjects();
	for (int i=0;i<numObjects;i++)
	{
		btCollisionObject* colObj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(colObj);
		const btVector3 pos = body->getCenterOfMassPosition();
		ofCircle(pos.getX(), pos.getY(), 20);
	}
	
	int numContainedObjects = containing->getNumChildShapes();
	for (int i=0;i<numContainedObjects;i++)
	{
		btCollisionShape* colObj = containing->getChildShape(i);
		btVector3* center = new btVector3(0.f, 0.f, 0.f );
		float radius;
		colObj->getBoundingSphere(*center, radius);
		std::cout << center->getX() << " " << center->getY() << " " << center->getZ() << std::endl;
		ofCircle(center->getX(), center->getY(), 10);
		delete center;
	}
	
}

void testApp::updateBullet()
{
	
	if (m_dynamicsWorld)
	{
		//btVector3	worldBoundsMin,worldBoundsMax;
		//getDynamicsWorld()->getBroadphase()->getBroadphaseAabb(worldBoundsMin,worldBoundsMax);
		//step the simulation
		m_dynamicsWorld->stepSimulation( ofGetElapsedTimeMillis() / 1000.f );
	}
}

void testApp::initBullet()
{
	
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* broadphase = new btSimpleBroadphase();
	
	btConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,constraintSolver,collisionConfiguration);
	
}

btSphereShape* testApp::createContainedBubble(ofVec3f origin, float radius, btCompoundShape* parent)
{
	
	btSphereShape* shape = new btSphereShape(radius);
	btTransform t;
	t.setOrigin(ofVec3fToBtVec(origin));
	//shape->setWorldTransform(t);
	
	btDefaultMotionState* motionstate = new btDefaultMotionState(t);
	btVector3 localInertia(0,0,0);
	btRigidBody::btRigidBodyConstructionInfo cInfo(1.0f, motionstate, shape, localInertia);
	btRigidBody* body = new btRigidBody(cInfo);
	
	m_dynamicsWorld->addRigidBody(body);
	return shape;
	
}


// Creates a compound shape
btCompoundShape* testApp::createContainingBubble(ofVec3f origin, ofVec2f size)
{	
	
	btVector3 btSize;
	btSize.setX( 1.f );
	btSize.setY( 1.f );
	btSize.setZ( 1.f );

	btCompoundShape* csh = new btCompoundShape();
	
	float angle = 0.f;
	float angleAdd = TWO_PI / ( size.x / 10.f );
	int numObjectsInBubble = size.x / 10;
	
	// add all shapes to define bounds
	// actually these don't seem to update the compoundshape
	for(int i = 0; i < numObjectsInBubble; i++)
	{
		btCollisionShape* shape = new btBoxShape( btSize );
		btTransform shapePos = btTransform::getIdentity();
		btVector3 bOrigin( 400, 400 + (i * 100),
					//origin.x + (sin(angle) * size.x), 
					//origin.y + (cos(angle) * size.y), 
					1.f
					);
		shapePos.setOrigin(bOrigin);
		
		csh->addChildShape(shapePos, shape);
		angle += angleAdd;
	}
	
	btTransform t;
	t.setIdentity();
	//t.setOrigin(ofVec3fToBtVec(origin));
	t.setOrigin(btVector3(100, 100, 0));
	
	btDefaultMotionState* motionstate = new btDefaultMotionState(t);
	btVector3 localInertia(0,0,0);
	btRigidBody::btRigidBodyConstructionInfo cInfo(1.0f, motionstate, csh, localInertia);
	//cInfo.m_startWorldTransform = t;
	btRigidBody* body = new btRigidBody(cInfo);
	
	csh->recalculateLocalAabb();
	
	m_dynamicsWorld->addRigidBody(body);
	
	return csh;
}

btVector4 testApp::ofVec4fToBtVec( ofVec4f ov ) {
	btVector4 bv(ov.x, ov.y, ov.z, ov.w);
	return bv;
}

btVector3 testApp::ofVec3fToBtVec( ofVec3f ov ) {
	btVector3 bv(ov.x, ov.y, ov.z);
	return bv;
}

ofVec4f testApp::btVecToOfVec4f( btVector4 bv ) {
	ofVec4f ov(bv.getX(), bv.getY(), bv.getZ(), bv.getW());
	return ov;
}

ofVec3f testApp::btVecToOfVec3f( btVector3 bv ) {
	ofVec3f ov(bv.getX(), bv.getY(), bv.getZ());
	return ov;
}
