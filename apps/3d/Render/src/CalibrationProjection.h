/**
 * CalibrationProjection.h
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#pragma once

#include "ProjectedRect.h"
#include "ofxCoordMapper.h"

class CalibrationProjection: public ProjectedRect {
public:
	CalibrationProjection();
	void draw();
	void setInteractiveArea(ofRectangle rect);
	bool drawInteractiveArea;
	bool drawFacets;
	bool drawLena;
	int facetColor;
	float lineWidth;
	float lenaScale;
private:
	
	ofImage lena;
	ofRectangle rect;
	ofxCoordMapper coordMapper;
};