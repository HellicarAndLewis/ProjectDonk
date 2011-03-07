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
#include "App.h"



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
		
	bullet.update();
	bubbleShader.update();
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

	
	// draw center of the app
	if(((Donk::App*)ofGetAppPtr())->guiEnabled){
		glLineWidth(3);
		ofNoFill();
		ofSetColor(255, 0, 0,128);
		glBegin(GL_LINES);
		glVertex2f(getWidth()/2-10, getHeight()/2);
		glVertex2f(getWidth()/2+10, getHeight()/2);
		glVertex2f(getWidth()/2, getHeight()/2-10);
		glVertex2f(getWidth()/2, getHeight()/2+10);
		glEnd();
	}
	
	
	//draw bubbles
	glPushMatrix();
	glTranslatef(0, 0, 0);
	for(int i=0; i<bubbles.size(); i++) {
		
		//billboarded layers
		bubbles[i]->drawHighLight();
		bubbles[i]->drawTwitterData();

		//shader sphere
		bubbles[i]->pushBubble();
		bubbleShader.begin();
		bubbles[i]->draw();
		bubbleShader.end();
		bubbles[i]->popBubble();
		
		
	}
	//bullet.drawFloor();
	glPopMatrix();
	
	
	// draw touches
	for(tIt = touches.begin(); tIt!=touches.end(); tIt++) {
		
		ofVec2f pos = mapToInteractiveArea((*tIt).second);
		//ofFill();
		ofSetColor(0,255,0,128);
		ofNoFill();
		glLineWidth(10);
		glPushMatrix();
		glTranslatef(pos.x,pos.y,0);
		glBegin(GL_LINES);
		glVertex2f(-20,   0);
		glVertex2f( 20,   0);
		glVertex2f(  0, -20);
		glVertex2f(  0,  20);
		glEnd();
		glPopMatrix();
		
		cout << "touch " << pos.x << " " << pos.y << endl;
		//for(int i=0; i<bubbles.size(); i++) {
		//	ofVec3f campos    = camera.getGlobalPosition();
		//	ofVec2f p =	bubbles[i]->rigidBody->getPosition() + campos;	
		//	ofLine(p, pos);
		//}
		
	}
	
	glLineWidth(1);	
	
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
