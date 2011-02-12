#include "testApp.h"


bool	callback::needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
{
	
	if( proxy0->m_collisionFilterGroup == 2 && proxy1->m_collisionFilterGroup == 2) {
		return false;
	}
	
	return true;
}

//--------------------------------------------------------------
void testApp::setup(){
	
	
	COL_NOTHING = 1;
	COL_SPHERE = 2;
	
	initBullet();
	ofVec3f origin(350, 400, 100);
	ofVec2f size(300, 300);
	createContainingBubble(origin, size);
	
	origin.x += 50;
	origin.y += 50;
	origin.z = 250;
	createContainedBubble(origin, 10.f);
	
	origin.x -= 50;
	origin.y -= 50;
	origin.z = 250;
	createContainedBubble(origin, 10.f);
	
	origin.x -= 50;
	origin.y -= 50;
	origin.z = 350;
	createContainedBubble(origin, 10.f);
	
	origin.x -= 100;
	origin.y += 50;
	origin.z = 250;
	createContainedBubble(origin, 10.f);
	
	origin.x -= 50;
	origin.y += 100;
	origin.z = 200;
	createContainedBubble(origin, 10.f);
	
	ofEnableAlphaBlending();
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
	/*vector<btRigidBody*>::iterator it = containedBubble.begin();
	while(it!=containedBubble.end())
	{
		
		btVector3 center = (*it)->getCenterOfMassPosition();
		
		btScalar yd = (float(y) - center.getY()) * 0.1;
		btScalar xd = (float(x) - center.getX()) * 0.1;
		
		cout << center.getY() << " " << center.getX() << " " << x << " " << y << " " << yd << " " << xd << endl;
		
		(*it)->applyImpulse(btVector3(xd, yd, 0), center);
		++it;
	}*/
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
	
	vector<btRigidBody*>::iterator it;
	
	ofSetColor(255, 0, 255, 100);
	it = containedBubble.begin();
	while( it != containedBubble.end())
	{
		const btVector3 pos = (*it)->getCenterOfMassPosition();
		ofCircle(pos.getX(), pos.getY(), (500 - pos.getZ())/20); // totally faking 3d
		++it;
	}
	
	ofSetColor(0, 0, 255, 51);
	it = containingBubble.begin();
	while( it != containingBubble.end())
	{
		const btVector3 pos = (*it)->getCenterOfMassPosition();
		ofCircle(pos.getX(), pos.getY(), (200 - pos.getZ())/50); // totally faking 3d
		//btTransform t = it->getWorldTransform();
		//btQuaternion q = t.getRotation();
		++it;
	}
	
}

void testApp::updateBullet()
{
	
	if (m_dynamicsWorld)
	{
		//step the simulation
		m_dynamicsWorld->stepSimulation( ofGetElapsedTimeMillis() / 100000.f );

		btCollisionObjectArray arr = m_dynamicsWorld->getCollisionObjectArray();
		for(int i = 0; i<arr.size(); i++)
		{
			btRigidBody* body = btRigidBody::upcast(arr[i]); 
			btCollisionShape* shape = body->getCollisionShape();
			if(shape->getShapeType() == 8)
			{
				//body->applyImpulse(btVector3(0.f, btScalar(0.5), 0), body->getCenterOfMassPosition());
				body->applyGravity();
			}
		}
	}
}

void testApp::initBullet()
{
	
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* broadphase = new btSimpleBroadphase();
	btConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver();
		
	m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,constraintSolver,collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, 10, 0));
	
	btOverlapFilterCallback * filterCallback = new callback();
	m_dynamicsWorld->getPairCache()->setOverlapFilterCallback(filterCallback);

	
}

btSphereShape* testApp::createContainedBubble(ofVec3f origin, float radius)
{
	
	btSphereShape* shape = new btSphereShape(radius);
	shape->setMargin(2);
	
	btTransform t;
	t.setOrigin(ofVec3fToBtVec(origin));
	//shape->setWorldTransform(t);
	
	btDefaultMotionState* motionstate = new btDefaultMotionState(t);
	btVector3 localInertia(0, 0.5, 0);

	shape->calculateLocalInertia(1.f, localInertia);
	
	btRigidBody::btRigidBodyConstructionInfo cInfo(1.f, motionstate, shape, localInertia);
	btRigidBody* body = new btRigidBody(cInfo);
	
	// inner bubbles have downwards gravity
	// will need to get these working so that sphere parts can cl
	//body->setCollisionFlags(COL_SPHERE);
	
	containedBubble.push_back(body);
	m_dynamicsWorld->addRigidBody(body);
	return shape;
	
}


// Creates a compound shape
void testApp::createContainingBubble(ofVec3f origin, ofVec2f size)
{	
	
	btVector3 btSize;
	btSize.setX( 50.f );
	btSize.setY( 1.f );//btSize.setY( 30.f );
	btSize.setZ( 50.f );
	
	btCollisionShape* shape = new btBoxShape( btSize );
	
	btVector3 localInertia( 0, 0, 0 );
	//shape->calculateLocalInertia(1.f,localInertia);
	
	btVector3 up(0, 1, 0);
	
	// create the sphere:
	for (int i = 1; i < 24; i++){
		for (int j = 0; j < 24; j++){
			
			btScalar cx = origin.x + (cos(i) * sin(j)*size.x );
			btScalar cy = origin.y + (cos(j)*size.x);
			btScalar cz = origin.z + (sin(i) * sin(j) * size.x);
			
			btVector3 pos(cx, cy, cz);
			btVector3 dist = pos - ofVec3fToBtVec(origin);
			dist.normalize();
			
			float theta = acos(dist.dot(up));
			btVector3 cross = up.cross(dist);
			cross.normalize();
			btQuaternion quat(cross, theta);
			
			btTransform t;
			t.setIdentity();
			t.setOrigin(pos);
			t.setRotation(quat);
			
			btDefaultMotionState* motionstate = new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo cInfo(1.f, motionstate, shape, localInertia);
			btRigidBody* spherePart = new btRigidBody(cInfo);
			
			// containing bubbles have upwards gravity
			spherePart->setCollisionFlags(1);
			
			containingBubble.push_back(spherePart);
			m_dynamicsWorld->addRigidBody(spherePart);
			//spherePart->activate(true);
			
		}
	}
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
