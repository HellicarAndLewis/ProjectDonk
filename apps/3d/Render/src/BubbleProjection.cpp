/**
 * BubbleProjection.cpp
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#include "BubbleProjection.h"
#include "testApp.h"
#include "Mode.h"
#include "AudioData.h"



//--------------------------------------------------------
BubbleProjection::BubbleProjection() {
	interactiveArea = ofRectangle(100,100,900,500);
}

//--------------------------------------------------------
/** This gets called straight after allocate() */
void BubbleProjection::setup() {	
	bullet.init();
	bullet.setGravity(0, 0, 0);
	bullet.camera = &camera;
	
	if(loadSphereShader()) {
		printf("Sphere shader loaded\n");
	}
	
}

//--------------------------------------------------------
int BubbleProjection::loadSphereShader() {
	
	int status = 0;
	// try setting up the gl stuff
	ofDisableArbTex();
	
	permImg.loadImage("shader/texturing.jpg");
	permImg.setImageType(OF_IMAGE_COLOR);
	glossImg.loadImage("shader/permutationTexture.jpg");
	glossImg.setImageType(OF_IMAGE_COLOR);
	ofEnableArbTex();
	
	//quadratic = gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	//gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	//gluQuadricTexture(quadratic, GL_TRUE);	
	
	status = shader.setup("shader/fresnel_refraction.vs", "shader/fresnel_refraction.fs");
	
	cubeMap.loadImages("shader/skybox/berkeley_positive_x.png",
					   "shader/skybox/berkeley_positive_y.png",
					   "shader/skybox/berkeley_positive_z.png",
					   "shader/skybox/berkeley_negative_x.png",
					   "shader/skybox/berkeley_negative_y.png",
					   "shader/skybox/berkeley_negative_z.png");
	
	
	return status;
}


//--------------------------------------------------------------
void BubbleProjection::beginSphere(){
	
	//GLint currentActiveTex;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTex);
	//float currentCoords[4];
	glGetFloatv(GL_CURRENT_TEXTURE_COORDS, &currentCoords[0]);
	
	//cout << currentActiveTex << " " << currentCoords[0] << " " << currentCoords[1] << " " << currentCoords[2] << " " << currentCoords[3] << " " << endl;
	
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	
	shader.begin();
	
	glActiveTexture(GL_TEXTURE8);
	unsigned int texId1 = permImg.getTextureReference().getTextureData().textureID;
	unsigned int texTarget1 = permImg.getTextureReference().getTextureData().textureTarget;  
	glBindTexture(texTarget1, texId1);
	
	glActiveTexture(GL_TEXTURE9);
	unsigned int texId2 = glossImg.getTextureReference().getTextureData().textureID;
	unsigned int texTarget2 = glossImg.getTextureReference().getTextureData().textureTarget;  
	glBindTexture(texTarget2, texId2);
	
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.textureObject);
	
	shader.setUniform1i("glossMap", 1);
	shader.setUniform1i("baseMap", 2);
	shader.setUniform1i("environmentMap", 3);
	
	shader.setUniform1f("EdgeFalloff", 0.2f);
	
	//float* eyeVector = new float[3];
	eyeVector[0] = ofGetWidth()/2;
	eyeVector[1] = ofGetHeight()/2;
	eyeVector[2] = 1;
	shader.setUniform3fv("eyeVector", eyeVector);
	
	//float* lpos = new float[3];
	lpos[0] = lightPosition.x;
	lpos[1] = lightPosition.y;
	lpos[2] = lightPosition.z;
	shader.setUniform3fv("lightVector", lpos);
	
	shader.setUniform1f("reflectAmount", 0.6f);
	
	//float* pos = new float[3];
	pos[0] = 0.5;
	pos[1] = 0.5;
	pos[2] = 0.5;
	shader.setUniform3fv("fresnelValues", pos);
	
	//float* cpos = new float[3];
	cpos[0] = modelview[3];//0.5;
	cpos[1] = modelview[7];//0.5;
	cpos[2] = modelview[11];//0.5;
	shader.setUniform3fv("CameraPos", cpos);
	
	//float* lPos = new float[3];
	lPos[0] = 0.5;
	lPos[1] = 0.5;
	lPos[2] = 0.5;
	shader.setUniform3fv("IoR_Values", lPos);
	
	shader.setUniform4mat("ModelWorld4x4", &modelview[0]);
	// gluSphere(quadratic, rigidBody->boxSize.getX(), rigidBody->boxSize.getX(), rigidBody->boxSize.getX());
	// gluSphere(quadratic, 100, 20, 20);
}

//--------------------------------------------------------------
void BubbleProjection::endSphere() {
	
	shader.end();
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
	
	glActiveTexture(GL_TEXTURE10);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	
	glActiveTexture(GL_TEXTURE9);
	glDisable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE8);
	glDisable(GL_TEXTURE_2D);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	
	//Reset texture 0
	glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	
	// global to content bubble now
	/*delete pos;
	 delete lPos;
	 delete eyeVector;
	 delete lpos;
	 */
	
	glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTex);
	glGetFloatv(GL_CURRENT_TEXTURE_COORDS, &currentCoords[0]);
	// cout << currentActiveTex << " " << currentCoords[0] << " " << currentCoords[1] << " " << currentCoords[2] << " " << currentCoords[3] << " " << endl;
	
	
}


//--------------------------------------------------------
/** Called every frame */
void BubbleProjection::update() {
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->update();	
		
		if(touches.size() == 0) {
			bubbles[i]->bTouched = false;			
		}
		
	}
	
	
	/*
	 btVector3 rayTo(pos.x, pos.y, 0);
	 btVector3 rayFrom = btVector3(campos.x, campos.y, campos.z);
	 
	 btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
	 bullet.world->rayTest(rayFrom, rayTo, rayCallback);
	 
	 if (rayCallback.hasHit()) {
	 printf("---");
	 btRigidBody * body = btRigidBody::upcast(rayCallback.m_collisionObject);
	 
	 btScalar m[16];
	 btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
	 myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
	 btVector3 org(m[12], m[13], m[14]);
	 
	 ofVec3f p1(rayTo.getX(), rayTo.getY(), rayTo.getZ());
	 ofVec3f p2(org.x(), org.y(), org.z());
	 ofSetColor(0, 255, 255);
	 ofLine(p1, p2);
	 
	 }
	 }
	 
	 */
	
	bullet.update();
}


//--------------------------------------------------------
void BubbleProjection::draw() {
	
	// clear the FBO
	ofEnableAlphaBlending();
	
	// audioReactiveness: 0 is not audio reactive, 1 is fully audio reactive
	float audioReactiveness = Donk::Mode::getInstance()->getValue("Background Audio-reactiveness");
	float volume = Donk::AudioData::getInstance()->getVolume(0);
	float amp = (1.f - audioReactiveness) + audioReactiveness*volume;//1 - volume *(1-audioReactiveness);
	ofClear(amp*Donk::Mode::getInstance()->getValue("Top BG Red"), 
			amp*Donk::Mode::getInstance()->getValue("Top BG Green"), 
			amp*Donk::Mode::getInstance()->getValue("Top BG Blue"), 255);
	
	// center of the app
	ofNoFill();
	ofSetColor(0, 255, 0);
	ofCircle(getWidth()/2, getHeight()/2, 10);
	
	//draw bubbles
	//glPushMatrix();
	//ofRectangle *rect = testApp::instance->calibrationProjection.rect;
	//glTranslatef(rect->x + rect->width/2,rect->y + rect->height/2,0);
	//Donk::BubbleData::render();
	//glPopMatrix();
	
	// --------------------------------------------
	// --------------------------------------------
	// --------------------------------------------
	//ofPushStyle();
	//camera.begin(ofRectangle(0, 0, getWidth(), getHeight()));
	
	
	
	// ---------------------
	// Bubbles
	// ---------------------
	glPushMatrix();
	glTranslatef(0, 0, 0);
	//glScalef(SCALE, SCALE, SCALE);
	
	for(int i=0; i<bubbles.size(); i++) {
	
		
		bubbles[i]->drawTwitterData();
		
		
		bubbles[i]->pushBubble();
		beginSphere();
		bubbles[i]->draw();	
		endSphere();
		bubbles[i]->popBubble();
		
		
	}

	
	
	//bullet.drawFloor();
	glPopMatrix();
	// ---------------------
	
	
	
	
	// this draws the touches - keep in here for now!
	for(tIt = touches.begin(); tIt!=touches.end(); tIt++) {
		
		ofVec2f pos = mapToInteractiveArea((*tIt).second);
		ofFill();
		ofSetColor(255, 0, 255);
		ofCircle(pos, 30);
		
		cout << pos.x << " " << pos.y << endl;
		//for(int i=0; i<bubbles.size(); i++) {
		//	ofVec3f campos    = camera.getGlobalPosition();
		//	ofVec2f p =	bubbles[i]->rigidBody->getPosition() + campos;	
		//	ofLine(p, pos);
		//}
		
		
		
	}
	
	
	// --------------------------------------------
	// --------------------------------------------
	// --------------------------------------------
	//camera.end();
	//ofPopStyle();
	
}



//--------------------------------------------------------
/** A bubble was just received */
void BubbleProjection::bubbleReceived(Donk::BubbleData *bubbleData) {
	printf("%s %s\n", bubbleData->text.c_str(), bubbleData->userName.c_str());
	
	ofVec3f center(getWidth()/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), getHeight(), ofRandom(-100, 100));
	float   radius = 80;
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = bubbleData;
	bubble->radius    = radius;
	bubble->rigidBody = bullet.createSphere(startPos, radius, 1);
	bubble->createContentBubble();
	bubble->target.set(center.x + ofRandom(-300, 300), ofRandom(500, getHeight()-300), 0);
	bubbles.push_back(bubble);
	
	
}

//--------------------------------------------------------
ofRectangle &BubbleProjection::getInteractiveArea() {
	return interactiveArea;
}


//--------------------------------------------------------
void BubbleProjection::addTouchConstraints(ContentBubble * bubble) {
	
	if(bubble->bTouched) return;
	TouchedConstraint * touchCon = new TouchedConstraint();
	touchCon->setTouchBody(bullet.world, bubble->rigidBody->body);
	touchConstraints.push_back(touchCon);
	printf("Constraint Made: %p\n", bubble);
	
}

//--------------------------------------------------------
void BubbleProjection::removeTouchConstraint(ContentBubble * bubble) {
	
	if(bubble == NULL) return;
	
	TouchedConstraint * touchCon = NULL;
	int					removeInd = -1;
	for (int i=0; i<touchConstraints.size(); i++) {
		if(touchConstraints[i]->body == bubble->rigidBody->body) {
			touchCon == touchConstraints[i];
			removeInd = i;
			printf("remove this cont\n");
		}
	}
	
	if(removeInd != -1) {
		touchConstraints[removeInd]->destroy();
		delete touchConstraints[removeInd];
		touchConstraints[removeInd] = NULL;
		touchConstraints.erase(touchConstraints.begin() + removeInd);
	}
	
	printf("Touch Constraint Size:%i\n", (int)touchConstraints.size());
}


//--------------------------------------------------------
void BubbleProjection::touchDown(float x, float y, int touchId) {
	
	
	ofVec2f touchCoords(x, y);
	ofVec2f pos = mapToInteractiveArea(touchCoords);
	
	
	
	for(int i=0; i<bubbles.size(); i++) {
		
		ofVec2f p1  = pos;
		ofVec2f p2  = bubbles[i]->rigidBody->getPosition();
		float	dis = p1.distance(p2);
		if(dis < bubbles[i]->radius + 50) {
			addTouchConstraints(bubbles[i]);
			bubbles[i]->bTouched = true;
		}
		else {
			bubbles[i]->bTouched = false;			
		}
	}
	
	
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

//--------------------------------------------------------
void BubbleProjection::touchMoved(float x, float y, int touchId) {
	touches[touchId] = ofVec2f(x, y);
	ofVec2f pos = mapToInteractiveArea(ofVec2f(x, y));
	
	for(int i=0; i<bubbles.size(); i++) {
		
		ofVec2f p1  = pos;
		ofVec2f p2  = bubbles[i]->rigidBody->getPosition();
		float	dis = p1.distance(p2);
		if(dis < bubbles[i]->radius+50 && !bubbles[i]->bTouched) {
			addTouchConstraints(bubbles[i]);
			bubbles[i]->bTouched = true;
		}
		
		
		for (int j=0; j<touchConstraints.size(); j++) {
			if(touchConstraints[j]->body == bubbles[i]->rigidBody->body) {
				touchConstraints[j]->updateTouchConstraint(pos);
			}
		}	
		
	}
	
	
	
}

//--------------------------------------------------------
void BubbleProjection::touchUp(float x, float y, int touchId) {
	
	ofVec2f pos = mapToInteractiveArea(ofVec2f(x, y));
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(bubbles[i]->bTouched) {
			bubbles[i]->bTouched = false;
			removeTouchConstraint(bubbles[i]);
		}
	}
	
	
	// remove old touches...
	if(touches.find(touchId)!=touches.end()) {
		touches.erase(touchId);
	}
}

//--------------------------------------------------------
ofVec2f BubbleProjection::mapToInteractiveArea(ofVec2f inPoint) {
	/*
	 ofVec2f pos = inPoint;
	 pos.x = ofMap(pos.x, 0.0, 1.0, -500, 500);
	 pos.y = ofMap(pos.y, 0.0, 1.0, -500, 500);
	 
	 printf("%f %f\n", pos.x, pos.y);
	 return pos;
	 */
	return ofVec2f(interactiveArea.x + interactiveArea.width  * inPoint.x,
				   interactiveArea.y + interactiveArea.height * inPoint.y);
}

//--------------------------------------------------------
void BubbleProjection::doubleTouchGesture(int touch1Id, int touch2Id) {
	ofVec2f doubleTouchCentre = (touches[touch1Id] + touches[touch2Id])/2;
	printf("doubleTouchGesture\n");
}
