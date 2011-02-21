/**
 * ofxCoordMapper.h
 * emptyExample
 *
 * Created by Marek Bereza on 18/02/2011.
 *
 */

/**
 * Usage:
 * call setup() (this creates the texture to be drawn onto the shape)
 * get the coordColorTexture, call beginCapture() draw your shape with normalized texCoords, call endCapture()
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
	
	void drawLUT();
private:
	int width, height;
	ofFbo coordColors;

	void drawColorsIntoTexture();
	
	// what gets generated from the coordinate pixelzz
	float *lut;
	ofFbo lutFbo;
	ofImage debugSaved;
};