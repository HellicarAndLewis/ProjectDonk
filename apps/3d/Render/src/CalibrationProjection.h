/**
 * CalibrationProjection.h
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#pragma once

#include "ProjectedRect.h"
#include "ofxXmlGui.h"

class CalibrationProjection: public ProjectedRect {
public:
	bool calibrate;
	CalibrationProjection();
	void draw();

	
	void setInteractiveArea(ofRectangle *rect);
	bool drawInteractiveArea;
	bool drawFacets;
	bool drawLena;
	bool drawChequerboard;
	int facetColor;
	int chequerboardColor;
	float lineWidth;
	float lenaScale;
	float chequerboardSize;
	//float maxParticleSize;
	//int particleSaturation;
	void createGui(ofxXmlGui *gui);
	float calibrationPointsInset;
	ofRectangle *rect;
private:
	void drawCrossHair(float x, float y);
	ofImage lena;
};