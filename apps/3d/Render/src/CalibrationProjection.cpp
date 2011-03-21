/**
 * CalibrationProjection.cpp
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#include "CalibrationProjection.h"
#include "constants.h"

CalibrationProjection::CalibrationProjection() {
	drawInteractiveArea = false;
	drawFacets = false;
	drawLena = false;
	drawChequerboard = false;
	facetColor = 0xFFFFFF;
	chequerboardColor = 0xFFFFFF;
	lineWidth = 1;
	lenaScale = 1;
	chequerboardSize = 100;
	lena.loadImage("lena.png");
	lena.setAnchorPercent(0.5, 0.5);
	calibrate = false;
	calibrationPointsInset = 0;
}

void CalibrationProjection::draw() {
	ofClear(0, 0, 0, 0);
	if(drawLena) {
		glPushMatrix();
		glTranslatef(getWidth()/2, getHeight()/2, 0);
		glScalef(lenaScale, lenaScale, 1);
		ofSetColor(255, 255, 255);
		lena.draw(0, 0);
		glPopMatrix();
	}
	if(drawChequerboard) {
		ofSetHexColor(chequerboardColor);
		for(int i = 0; i < getWidth(); i+=chequerboardSize) {
			for(int j = 0; j < getHeight(); j+=chequerboardSize) {
				if((i+j)%2==0) {
					ofRect(i, j, chequerboardSize, chequerboardSize);
				}
			}
		}
	}
	   
	   
	if(drawInteractiveArea) {
		ofSetHexColor(0x990000);
		ofRect(*rect);
		
		
		float hInset = rect->width*calibrationPointsInset;
		float vInset = rect->height*calibrationPointsInset;
		drawCrossHair(rect->x+hInset, rect->y+vInset);
		drawCrossHair(rect->x+rect->width - hInset, rect->y+vInset);
		drawCrossHair(rect->x+hInset, rect->y + rect->height - vInset);
		drawCrossHair(rect->x+rect->width - hInset, rect->y + rect->height - vInset);
	}
	
}

void CalibrationProjection::drawCrossHair(float x, float y) {
	float rad = getWidth()/80;
	glLineWidth(6);
	ofSetHexColor(0xFFFFFF);
	ofNoFill();
	ofCircle(x, y, rad);
	ofSetHexColor(0x00FF00);
	rad *= 0.68;
	ofLine(x-rad, y, x+rad, y);
	ofLine(x, y-rad, x, y+rad);
	ofFill();
	glLineWidth(1);
}

void CalibrationProjection::setInteractiveArea(ofRectangle *rect) {
	this->rect = rect;
}

void CalibrationProjection::createGui(ofxXmlGui *gui) {
	gui->addToggle("Calibration Mode", calibrate);
	gui->addTitle("Projection");
	gui->addToggle("Draw Lena", drawLena);
	gui->addSlider("Lena scale", lenaScale, 0.2, 5);
	gui->addToggle("Draw Chequerboard", drawChequerboard);
	gui->addSlider("Chequerboard size", chequerboardSize, 10, 400);
	gui->addColorPicker("Chequerboard Color", chequerboardColor);
	gui->addToggle("Draw Facets", drawFacets);
	gui->addColorPicker("Facet Color", facetColor);
	gui->addSlider("Line Width", lineWidth, 1, 10);
	gui->addTitle("Interaction Area");
	gui->addToggle("Draw Interactive Area", drawInteractiveArea);
	gui->addSlider("Interaction x", rect->x, 0, PROJECTION_RESOLUTION_WIDTH);
	gui->addSlider("Interaction y", rect->y, 0, PROJECTION_RESOLUTION_WIDTH);
	gui->addSlider("Interaction width", rect->width, 0, PROJECTION_RESOLUTION_WIDTH);
	gui->addSlider("Interaction height", rect->height, 0, PROJECTION_RESOLUTION_HEIGHT);
	gui->addSlider("calibration points inset", calibrationPointsInset, 0, 0.4);
	//gui->addSlider("particle size", maxParticleSize, 0, 96.0);
	//gui->addSlider("particle hue", particleSaturation, 0, 255);
	gui->enableAutoSave("settings/interactionSettings.xml");
	calibrate = false;
}

