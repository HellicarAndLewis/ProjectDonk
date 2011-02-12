/*
 *  Bubble.h
 *  audioReactive
 *
 *  Created by Marek Bereza on 19/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
class Bubble {
public: 
	ofColor color;
	float brightness;
	Bubble() {
		color.setHsb(ofRandom(0, 360), 190, 255, 255);
		if(!inited) {
			init();
		}
		brightness = ofRandom(0.5, 1.0);
		radius = radiusBase = ofRandom(10, 50);
		vel = ofVec2f(0.f, ofRandom(-1, 1));
		seed = ofRandom(0.4, 1.3);
	}
	float seed;
	void update() {
		pos += vel;
		vel += ofVec2f(
					  ofRandom(-0.2, 0.2),
					  0
		);
		radius = radiusBase + (radiusBase *0.4)*sin(seed+ofGetElapsedTimef()*seed);
	}
	void draw() {
//		printf("Bubble\n");
		ofFill();
		ofSetColor(color);
		ofCircle(pos.x, pos.y, radius);
//		ofSetColor(color);
//		img.draw(pos.x, pos.y, radius*2, radius*2);
	}
	
	
	float radius;
	float radiusBase;
	ofVec2f pos;
	ofVec2f vel;
	
	static ofImage img;
	static void init() {
		if(!img.loadImage("bubble.png")) {
			printf("Couldn't load image\n");
		}
		img.setAnchorPercent(0.5, 0.5);
		inited = true;
	}
	static bool inited;
};