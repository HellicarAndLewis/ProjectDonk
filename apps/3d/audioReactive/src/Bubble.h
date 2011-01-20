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
		color.setHsb(ofRandom(110, 150), 50, 255, 255);
		if(!inited) {
			init();
		}
		brightness = ofRandom(0.5, 1.0);
		radius = ofRandom(10, 50);
		vel = ofVec2f(0.f, -1);
		seed = ofRandom(0.001, 0.002);
	}
	float seed;
	void update() {
		pos += vel;
		vel += ofVec2f(
					  ofRandom(-0.2, 0.2),
					  0
		);

	}
	void draw() {
		ofFill();
		ofSetColor(color.r, color.g, color.b, 40*brightness);
		ofCircle(pos.x, pos.y, radius);
		ofSetColor(color);
		img.draw(pos.x, pos.y, radius*2, radius*2);
	}
	
	
	float radius;
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