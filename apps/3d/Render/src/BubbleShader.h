/*
 *  BubbleShader.h
 *  Render
 *
 *  Created by Josh Nimoy on 3/6/11.
 ported from code by Joshua Noble
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxCubeMap.h"
#include <GLUT/glut.h>


class BubbleShader{
	
public:
	
	BubbleShader();
	void update();
	void begin();
	void end();
	
	void renderGluSphereWithTexCoords(float x, float y, float z, float radius);
	
	unsigned int permTexture;
	
	GLUquadricObj *quadratic;
	
	ofShader cubeshader;
	ofxCubeMap cubeMap;
	
	ofVec3f sphereCenter;
	ofEasyCam cam;
	
	ofImage permImg;
	ofImage glossImg;
	
	ofTexture permTex, glossTex;
	
	ofPixels permPixels;
	ofPixels glossPixels;
	
	float xLightPos,yLightPos,zLightPos;
	bool inc;
	
	bool hasDrawnTex;
	
	ofFbo first;
	ofFbo second;
	
};
