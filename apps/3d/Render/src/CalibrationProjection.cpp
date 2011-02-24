/**
 * CalibrationProjection.cpp
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#include "CalibrationProjection.h"
CalibrationProjection::CalibrationProjection() {
	coordMapper.setup();
	drawInteractiveArea = false;
	drawFacets = false;
	drawLena = false;
	facetColor = 0xFFFFFF;
	lineWidth = 1;
	lenaScale = 1;
	lena.loadImage("lena.png");
	lena.setAnchorPercent(0.5, 0.5);
}

void CalibrationProjection::draw() {
	ofClear(0, 0, 0, 0);
	if(drawLena) {
		glPushMatrix();
		glScalef(lenaScale, lenaScale, 1);
		ofSetColor(255, 255, 255);
		lena.draw(getWidth()/2, getHeight()/2);
		glPopMatrix();
	}
	if(drawInteractiveArea) {
		ofSetHexColor(0xFFFFFF);
		coordMapper.getCoordTexture()->draw(rect.x, rect.y, rect.width, rect.height);
	}
}

void CalibrationProjection::setInteractiveArea(ofRectangle rect) {
	this->rect = rect;
}