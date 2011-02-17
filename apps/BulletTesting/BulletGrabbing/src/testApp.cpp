#include "testApp.h"
#include "btConvexPointCloudShape.h"

ofxBulletRigidBody * bd;

//--------------------------------------------------------------
void testApp::setup() {
	
	bApplyForce = true;
	ofSetSphereResolution(4);
	ofBackground(0);
	
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

	// this is the camera we are using for bullet.
	camera.resetTransform();
	camera.setFov(60);
	camera.clearParent();
	
	camera.setPosition(ofVec3f(0, 0, 500));	
	camera.orbit(90, -90, 500);
	
	
	bd = bullet.createSphere(ofVec3f(0, 200, 0), 30, 1);
	for (int i=0; i<10; i++) {
		ofVec3f pos(ofRandom(-50, 50), ofRandom(0, 150), ofRandom(-50, 50));
		bullet.createSphere(pos, ofRandom(8, 15), 1);
	}
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
void testApp::drawViaBulletGL() {
	
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
void testApp::drawViaCustomGL() {
	
	
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
		
		/*
		 void GL_ShapeDrawer::drawSphere(btScalar radius, int lats, int longs) 
		 {
		 int i, j;
		 for(i = 0; i <= lats; i++) {
		 btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
		 btScalar z0  = radius*sin(lat0);
		 btScalar zr0 =  radius*cos(lat0);
		 
		 btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
		 btScalar z1 = radius*sin(lat1);
		 btScalar zr1 = radius*cos(lat1);
		 
		 glBegin(GL_QUAD_STRIP);
		 for(j = 0; j <= longs; j++) {
		 btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
		 btScalar x = cos(lng);
		 btScalar y = sin(lng);
		 glNormal3f(x * zr1, y * zr1, z1);
		 glVertex3f(x * zr1, y * zr1, z1);
		 glNormal3f(x * zr0, y * zr0, z0);
		 glVertex3f(x * zr0, y * zr0, z0);
		 }
		 glEnd();
		 }
		 } 
		 */
		
		ofSetColor(255, 255, 255);
		texture.getTextureReference().bind();
		drawSphere(ofVec3f(0, 0, 0), radius, 8);
		texture.getTextureReference().unbind();
		
		glPopMatrix();
		
		//		printf("aabbMin=(%f,%f,%f)\n",aabbMin.getX(),aabbMin.getY(),aabbMin.getZ());
		//		printf("aabbMax=(%f,%f,%f)\n",aabbMax.getX(),aabbMax.getY(),aabbMax.getZ());
		//		m_dynamicsWorld->getDebugDrawer()->drawAabb(aabbMin,aabbMax,btVector3(1,1,1));
		
		
		//switch(pass) {
				//case	0:	drawer.drawOpenGL(m,colObj->getCollisionShape(),wireColor, getDebugMode(), aabbMin,aabbMax);break;
				//case	1:	drawer.drawShadow(m,m_sundirection*rot,colObj->getCollisionShape(),aabbMin,aabbMax);break;
		//	case	2:	drawer.drawOpenGL(m,colObj->getCollisionShape(),wireColor*0.3,0,aabbMin,aabbMax);break;
		//}
		
	}
	
	
}

//--------------------------------------------------------------
void testApp::drawSphere( const ofVec3f &center, float radius, int segments ) {
	
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
void testApp::draw() {
	
	
	
	//btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
	//getWorld()->rayTest(rayFrom,rayTo,rayCallback);
	//if (rayCallback.hasHit()) {
		
	
	light.resetTransform();
	light.setPosition(ofVec3f(0, 0, 0));
	light.orbit(90, -90, 500);

	

	
	camera.begin();

	light.enable();
	light.draw();
	camera.draw();

	
	
	//glPushMatrix();
	//glTranslatef(ofGetWidth()/2, (ofGetHeight()/2), 40);
	//glScaled(SCALE, SCALE, SCALE);
	//ofRotateX(90);
	//ofRotateY(0);
	
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
	
	/*
	btVector3 rayTo   = bullet.getRayTo(mouseX, mouseY, &camera);
	ofVec3f   camPos  = camera.getPosition();
	btVector3 rayFrom = btVector3(camPos.x, camPos.y, camPos.z);
	
//	printf("TO: %f,%f,%f\n", rayTo.x(), rayTo.y(), rayTo.z());
	
	ofVec3f rayT(rayTo.x(), rayTo.y(), rayTo.z());
	ofVec3f rayF(rayFrom.x(), rayFrom.y(), rayFrom.z());
	
	ofPushStyle();
	glLineWidth(3);
	ofSetColor(255, 0, 0);
	ofLine(rayT, rayF);
	ofPopStyle();
	*/
	
	
	
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
	
	if(key == 'b') {
		
		ofVec3f pos(ofRandom(-50, 50), ofRandom(0, 150), ofRandom(-50, 50));
		bullet.createSphere(pos, ofRandom(5, 10), 1);
		
	}
	if(key == 'B') {
		
		ofVec3f pos(ofRandom(-50, 50), ofRandom(0, 150), ofRandom(-50, 50));
		bullet.createSphere(pos, 50, 1);
		
	}
	
	if(key == '1') {
		btRigidBody * body = btRigidBody::upcast(bd->body);
		btSphereShape* sphereShape = static_cast<const btSphereShape*>(body->getCollisionShape());
		sphereShape->setUnscaledRadius((btScalar)ofRandom(10, 140));
	}
	
	/*if(key == ' ') {
	
		//bullet.createCone(ofVec3f(0, 800, 0), 9, 9, 1);
		
		ofVec3f pos(0, 400, 0);
		int     mass = 1;
		
		int nPts = 5;
		ofVec3f pts[nPts];
		
		pts[0].set(-50, -50, 0);	pts[1].set(50, -50, 0);
		pts[2].set(50, 50, 0);		pts[3].set(0, 0, 50);
		pts[4].set(-50, -50, 50);
		
		btVector3 * btPts = ofVec3TobtVec3(pts, nPts);
		
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(pos.x/SCALE, pos.y/SCALE, pos.z/SCALE));
		
		ofxBulletRigidBody * b = new ofxBulletRigidBody();
		b->world = bullet.world;
		//b->shape = new btConvexHullShape();
		
		//btConvexHullShape * hull = (btConvexHullShape*)b->shape;
		for (int i=0; i<nPts; i++) {
			btPts[i] = btVector3(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100));
			btPts[i] /= SCALE;
			//btVector3 p = tobtVec3(pts[i]);
			//p /= SCALE;
			//hull->addPoint(p);
		}
		
		b->shape = new btConvexPointCloudShape(btPts, nPts, btVector3(10, 10, 10));
		b->createRigidBody(mass, startTransform);
		
		bullet.world->addRigidBody(b->body);
		bullet.rigidBodies.push_back(b);
		
	}*/
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	bullet.mouseDragged(x, y);
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	bullet.mousePressed(x, y);
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	bullet.mouseReleased(x, y);
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}
