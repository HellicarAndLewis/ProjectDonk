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

void BubbleProjection::draw() {
	ofClear(0, 100, 0, 0);
	
	map<int,ofVec2f>::iterator it;
	ofSetHexColor(0xFF0000);
	for(it = touches.begin(); it!=touches.end(); it++) {
		ofVec2f pos = mapToInteractiveArea((*it).second);
		ofCircle(pos, 10);
		printf("%f %f\n", pos.x, pos.y);
	}
}

ofRectangle &BubbleProjection::getInteractiveArea() {
	return interactiveArea;
}

void BubbleProjection::touchDown(float x, float y, int touchId) {
	touches[touchId] = ofVec2f(x, y);
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
				   interactiveArea.y + interactiveArea.height - interactiveArea.height * inPoint.y);
}