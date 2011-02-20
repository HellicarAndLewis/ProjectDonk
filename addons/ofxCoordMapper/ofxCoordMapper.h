/**
 * ofxCoordMapper.h
 * emptyExample
 *
 * Created by Marek Bereza on 18/02/2011.
 *
 */

/**
 * Usage:
 * get the coordColorTexture, call begin() draw the texture to your shape, call end()
 */
#pragma once

#include "ofMain.h"

class ofxCoordMapper {
public:
	ofxCoordMapper();
	~ofxCoordMapper();
	void setup();
	void beginCapture();
	void endCapture();
private:
	int width, height;
	ofFbo coordColors;

	void drawColorsIntoTexture();
	
	// what gets generated from the coordinate pixelzz
	float *lut;
	ofFbo lutFbo;
	ofImage debugSaved;
};