/**
 * BubbleProjection.cpp
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#include "BubbleProjection.h"

BubbleProjection::BubbleProjection() {
	interactiveArea = ofRectangle(100,100,900,500);
}

void BubbleProjection::setup() {
	ofSetSphereResolution(4);
	ofEnableLighting();
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
	light.setDirectional(false);
	light.setAmbientColor(ofColor(100, 120, 100));
	light.setDiffuseColor(ofColor(255, 255, 255));
	
	
	bullet.init();
	bullet.createGround(200.0);
	bullet.world->setGravity(btVector3(0, -5, 0));
	bullet.camera = &camera;
	
	// wicked texture!
	ofDisableArbTex();
	texture.loadImage("image.png");
	ofEnableArbTex();
	// this is the camera we are using for bullet.
	camera.resetTransform();
	camera.setFov(60);
	camera.clearParent();
	
	camera.setPosition(ofVec3f(0, 0, 500));	
	camera.orbit(90, -90, 500);
	
	
	bullet.createSphere(ofVec3f(0, 200, 0), 30, 1);
	for (int i=0; i<50; i++) {
		ofVec3f pos(ofRandom(-50, 50), ofRandom(0, 150), ofRandom(-50, 50));
		bullet.createSphere(pos, ofRandom(8, 15), 1);
	}
	
}
















































//--------------------------------------------------------------
void BubbleProjection::update() {
	
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
			//b->setDamping(0.74, 0.74);
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
void BubbleProjection::drawViaBulletGL() {
	
	int pass = 2;
	
	btScalar	m[16];
	btMatrix3x3	rot;rot.setIdentity();
	const int	numObjects = bullet.world->getNumCollisionObjects();
	btVector3 wireColor(1,0,0);
	for(int i=0;i<numObjects;i++)
	{
		btCollisionObject*	colObj=bullet.world->getCollisionObjectArray()[i];
		btRigidBody*		body=btRigidBody::upcast(colObj);
		if(body&&body->getMotionState())
		{
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			rot=myMotionState->m_graphicsWorldTrans.getBasis();
		}
		else
		{
			colObj->getWorldTransform().getOpenGLMatrix(m);
			rot=colObj->getWorldTransform().getBasis();
		}
		btVector3 wireColor(1.f,1.0f,0.5f); //wants deactivation
		if(i&1) wireColor=btVector3(0.f,0.0f,1.f);
		///color differently for active, sleeping, wantsdeactivation states
		if (colObj->getActivationState() == 1) //active
		{
			if (i & 1)
			{
				wireColor += btVector3 (1.f,0.f,0.f);
			}
			else
			{			
				wireColor += btVector3 (.5f,0.f,0.f);
			}
		}
		if(colObj->getActivationState()==2) //ISLAND_SLEEPING
		{
			if(i&1)
			{
				wireColor += btVector3 (0.f,1.f, 0.f);
			}
			else
			{
				wireColor += btVector3 (0.f,0.5f,0.f);
			}
		}
		
		btVector3 aabbMin,aabbMax;
		bullet.world->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
		
		aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
		aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
		//		printf("aabbMin=(%f,%f,%f)\n",aabbMin.getX(),aabbMin.getY(),aabbMin.getZ());
		//		printf("aabbMax=(%f,%f,%f)\n",aabbMax.getX(),aabbMax.getY(),aabbMax.getZ());
		//		m_dynamicsWorld->getDebugDrawer()->drawAabb(aabbMin,aabbMax,btVector3(1,1,1));
		
		
		switch(pass) {
				//case	0:	drawer.drawOpenGL(m,colObj->getCollisionShape(),wireColor, getDebugMode(), aabbMin,aabbMax);break;
				//case	1:	drawer.drawShadow(m,m_sundirection*rot,colObj->getCollisionShape(),aabbMin,aabbMax);break;
			case	2:	drawer.drawOpenGL(m,colObj->getCollisionShape(),wireColor*0.3,0,aabbMin,aabbMax);break;
		}
		
	}
}

//--------------------------------------------------------------
void BubbleProjection::drawViaCustomGL() {
	
	
	btScalar	m[16];
	btMatrix3x3	rot;
	rot.setIdentity();
	
	const int	numObjects = bullet.world->getNumCollisionObjects();
	
	
	for(int i=0; i<numObjects; i++) {
		
		btCollisionObject*	colObj=bullet.world->getCollisionObjectArray()[i];
		btRigidBody*		body=btRigidBody::upcast(colObj);
		if(body && body->getMotionState()) {
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			rot=myMotionState->m_graphicsWorldTrans.getBasis();
		}
		else {
			colObj->getWorldTransform().getOpenGLMatrix(m);
			rot = colObj->getWorldTransform().getBasis();
		}
		
		
		btVector3 aabbMin, aabbMax;
		bullet.world->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
		aabbMin -= btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
		aabbMax += btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
		
		
		
		glPushMatrix(); 
		glMultMatrixf(m);
		
		
		const btSphereShape* sphereShape = static_cast<const btSphereShape*>(colObj->getCollisionShape());
		float radius = sphereShape->getMargin();
		
				
		ofSetColor(255, 255, 255);
		texture.getTextureReference().bind();
		drawSphere(ofVec3f(0, 0, 0), radius, 8);
		texture.getTextureReference().unbind();
		
		glPopMatrix();
		
	
		
	}
	
	
}

//--------------------------------------------------------------
void BubbleProjection::drawSphere( const ofVec3f &center, float radius, int segments ) {
	
	if( segments < 0 ) return;
	
	float *verts = new float[(segments+1)*2*3];
	float *normals = new float[(segments+1)*2*3];
	float *texCoords = new float[(segments+1)*2*2];
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, verts );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );
	glEnableClientState( GL_NORMAL_ARRAY );
	glNormalPointer( GL_FLOAT, 0, normals );
	
	for( int j = 0; j < segments / 2; j++ ) {
		float theta1 = j * 2 * 3.14159f / segments - ( 3.14159f / 2.0f );
		float theta2 = (j + 1) * 2 * 3.14159f / segments - ( 3.14159f / 2.0f );
		
		for( int i = 0; i <= segments; i++ ) {
			ofVec3f e, p;
			float theta3 = i * 2 * 3.14159f / segments;
			
			e.x = cos( theta1 ) * cos( theta3 );
			e.y = sin( theta1 );
			e.z = cos( theta1 ) * sin( theta3 );
			p = e * radius + center;
			normals[i*3*2+0] = e.x; normals[i*3*2+1] = e.y; normals[i*3*2+2] = e.z;
			texCoords[i*2*2+0] = 0.999f - i / (float)segments; texCoords[i*2*2+1] = 0.999f - 2 * j / (float)segments;
			verts[i*3*2+0] = p.x; verts[i*3*2+1] = p.y; verts[i*3*2+2] = p.z;
			
			e.x = cos( theta2 ) * cos( theta3 );
			e.y = sin( theta2 );
			e.z = cos( theta2 ) * sin( theta3 );
			p = e * radius + center;
			normals[i*3*2+3] = e.x; normals[i*3*2+4] = e.y; normals[i*3*2+5] = e.z;
			texCoords[i*2*2+2] = 0.999f - i / (float)segments; texCoords[i*2*2+3] = 0.999f - 2 * ( j + 1 ) / (float)segments;
			verts[i*3*2+3] = p.x; verts[i*3*2+4] = p.y; verts[i*3*2+5] = p.z;
		}
		glDrawArrays( GL_TRIANGLE_STRIP, 0, (segments + 1)*2 );
	}
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	
	delete [] verts;
	delete [] normals;
	delete [] texCoords;
}






//--------------------------------------------------------------
void BubbleProjection::draw() {
	
	
	ofEnableLighting();
	ofClear(0, 100, 0, 0);

	
	light.resetTransform();
	light.setPosition(ofVec3f(0, 0, 0));
	light.orbit(90, -90, 500);
	
	
	
	
	camera.begin();
	
	light.enable();
	light.draw();
	camera.draw();
	
	
	
	
	glPushMatrix();
	glScaled(SCALE, SCALE, SCALE);
	ofSetColor(25, 225, 125);
	//bullet.draw();
	//drawViaBulletGL();
	drawViaCustomGL();
	ofSetColor(125, 125, 125);
	bullet.drawFloor();
	glPopMatrix();
	light.disable();
	
	
	
	camera.end();

	
	
	ofDisableLighting();
	
	ofSetHexColor(0xFF0000);
	for(tIt = touches.begin(); tIt!=touches.end(); tIt++) {
		ofVec2f pos = mapToInteractiveArea((*tIt).second);
		ofCircle(pos, 30);
	}

	
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 1));
	
}








































ofRectangle &BubbleProjection::getInteractiveArea() {
	return interactiveArea;
}

void BubbleProjection::touchDown(float x, float y, int touchId) {
	

	ofVec2f touchCoords(x, y);
	
	// find the closest point to the new touch
	float minSqrDist = FLT_MAX; // do squares
	int minTouchId = -1;

	for(tIt = touches.begin(); tIt!=touches.end(); tIt++) {
		float sqrDist = touchCoords.squareDistance((*tIt).second);
		if(sqrDist<minSqrDist) {
			minTouchId = (*tIt).first;
			minSqrDist = sqrDist;
		}
	}
	
	// the minimum distance between the 2 closest touches 
	// in order for it to be a double touch.
	float doubleTouchDist = 0.1;
	
	// add the touch
	touches[touchId] = touchCoords;
	// if there's another touch, and it's close enough, call doubleTouchGesture
	if(minTouchId!=-1 && sqrt(minSqrDist)<doubleTouchDist) { 
		doubleTouchGesture(touchId, minTouchId);
		
	}
	
}
void BubbleProjection::touchMoved(float x, float y, int touchId) {
	touches[touchId] = ofVec2f(x, y);
}

void BubbleProjection::touchUp(float x, float y, int touchId) {
	if(touches.find(touchId)!=touches.end()) {
		touches.erase(touchId);
	}
}

ofVec2f BubbleProjection::mapToInteractiveArea(ofVec2f inPoint) {
	return ofVec2f(interactiveArea.x + interactiveArea.width * inPoint.x,
				   interactiveArea.y + interactiveArea.height * inPoint.y);
}

void BubbleProjection::doubleTouchGesture(int touch1Id, int touch2Id) {
	

	ofVec2f doubleTouchCentre = (touches[touch1Id] + touches[touch2Id])/2;
	printf("doubleTouchGesture\n");
}
