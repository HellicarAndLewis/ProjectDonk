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
	for (int i=0;i<numObjects;i++)
	{
		btCollisionShape* colObj = containing->getChildShape(i);
		btVector3* center = new btVector3(0.f, 0.f, 0.f );
		float radius;
		colObj->getBoundingSphere(*center, radius);
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
	
	//btConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	//btOverlappingPairCache* broadphase = new btSimpleBroadphase();
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

/*btRigidBody* testApp::localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
	
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	
	btVector3 localInertia(0,0,0);
	if (isDynamic) {
		shape->calculateLocalInertia(mass,localInertia);
	}
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);
	
	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
	
	m_dynamicsWorld->addRigidBody(body);
	
	return body;
}*/


// Creates a compound shaped tower centered at the ground level
btCompoundShape* testApp::createContainingBubble(ofVec3f origin, ofVec2f size)
{	
	
	btVector3 btSize;
	btSize.setX( 10.f );
	btSize.setY( 10.f );
	btSize.setZ( 10.f );

	btScalar collisionMargin(0.0);
	btCompoundShape* csh = new btCompoundShape();
	
	
	float angleAdd = TWO_PI / ( size.x / 10.f );
	int numObjectsInBubble = size.x / 10;
	float angle = 0.f;
	/*// add all points to define shape
	for(int j = 0; j < numObjectsInBubble; j++)
	{
		btVector3 v(origin.x + (sin(angle) * size.x), (origin.y + (cos(angle) * size.y), 1.f);
		csh->addPoint(v);
		angle += angleAdd;
	}*/
	
	angle = 0.f;
	
	// add all shapes to define bounds
	for(int i = 0; i < numObjectsInBubble; i++)
	{
		btCollisionShape* shape = new btBoxShape( btSize );
		btTransform shapePos = btTransform::getIdentity();
		btVector3 bOrigin( 
					origin.x + (sin(angle) * size.x), 
					origin.y + (cos(angle) * size.y), 
					1.f
					);
		shapePos.setOrigin(bOrigin);
		
		csh->addChildShape(shapePos, shape);
		angle += angleAdd;
	}
	
	btTransform t;
	t.setOrigin(ofVec3fToBtVec(origin));
	
	btDefaultMotionState* motionstate = new btDefaultMotionState(t);
	btVector3 localInertia(0,0,0);
	btRigidBody::btRigidBodyConstructionInfo cInfo(1.0f, motionstate, csh, localInertia);
	cInfo.m_startWorldTransform = t;
	btRigidBody* body = new btRigidBody(cInfo);
	
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
