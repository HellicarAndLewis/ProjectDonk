/**
 * BubbleProjection.cpp
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#include "BubbleProjection.h"
#include "testApp.h"



//--------------------------------------------------------
BubbleProjection::BubbleProjection() {
	interactiveArea = ofRectangle(100,100,900,500);
}



//--------------------------------------------------------
/** This gets called straight after allocate() */
void BubbleProjection::setup() {
	

	
	light.setDirectional(false);
	light.setAmbientColor(ofColor(100, 120, 100));
	light.setDiffuseColor(ofColor(255, 255, 255));
	
	
	camera.resetTransform();
	camera.setPosition(ofVec3f(0, 0, 0));
	camera.orbit(0, 0, 500);

	
	bullet.init();
	//bullet.createGround(50);
	bullet.setGravity(0, 0, 0);
	bullet.camera = &camera;
	
	
}

//--------------------------------------------------------
/** Called every frame */
void BubbleProjection::update() {
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->update();	
	}
	
	bullet.update();
}


//--------------------------------------------------------
void BubbleProjection::draw() {
	// clear the FBO
	ofClear(0, 50, 0, 0);

	//draw bubbles
	//glPushMatrix();
	//ofRectangle *rect = testApp::instance->calibrationProjection.rect;
	//glTranslatef(rect->x + rect->width/2,rect->y + rect->height/2,0);
	//Donk::BubbleData::render();
	//glPopMatrix();
	
	
	// this draws the touches - keep in here for now!
	ofSetHexColor(0xFF0000);
	for(tIt = touches.begin(); tIt!=touches.end(); tIt++) {
		ofVec2f pos = mapToInteractiveArea((*tIt).second);
		ofCircle(pos, 30);
		
	
		
	}
	
	// center of the app
	ofVec2f pos(getWidth()/2, getHeight()/2);
	ofCircle(pos, 10);
	

	ofEnableLighting();
	light.enable();
	camera.begin();
	light.draw();
	camera.draw();
	
	// ---------------------
	glPushMatrix();
	glScaled(SCALE, SCALE, SCALE);
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->draw();	
	}
	
	//bullet.drawFloor();
	
	// world bounds
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofNoFill();
	ofSetColor(255, 255, 0);
	ofRect(0, 0, 550, 500);
	ofSetRectMode(OF_RECTMODE_CORNER);
	
	glPopMatrix();
	// ---------------------
	
	
	light.disable();
	camera.end();
	ofDisableLighting();
	
	
}



//--------------------------------------------------------
/** A bubble was just received */
void BubbleProjection::bubbleReceived(Donk::BubbleData *bubbleData) {
	printf("%s %s\n", bubbleData->text.c_str(), bubbleData->userName.c_str());
	
	ofVec3f center(0, 0, 0);
	ofVec3f startPos(0, 500, 0);
	float   radius = 50;
	
	ContentBubble * bubble = new ContentBubble();
	bubble->data = bubbleData;
	bubble->rigidBody = bullet.createSphere(startPos, radius, 1);
	bubble->createContentBubble();
	bubble->target.set(ofRandom(-200, 200), ofRandom(-200, 200), 0);
	bubbles.push_back(bubble);
	
	
}

//--------------------------------------------------------
ofRectangle &BubbleProjection::getInteractiveArea() {
	return interactiveArea;
}

//--------------------------------------------------------
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

//--------------------------------------------------------
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
