/*
 *  Bubbles.h
 *  audioReactive
 *
 *  Created by Marek Bereza on 19/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */
#pragma once
#include "Bubble.h"
#include "ProjectedRect.h"

class Bubbles: public ProjectedRect {
public:

	void spawn(ofVec2f pos) {
		bubbles.push_back(Bubble());
		bubbles.back().pos = pos;
		
	}
	void collision(Bubble &bubble1, Bubble &bubble2) {
		float minDistSqr = bubble1.radius + bubble2.radius;
		minDistSqr *= minDistSqr;
		float currDistSqr = bubble2.pos.squareDistance(bubble1.pos);
		
		// there's a collision
		if(minDistSqr>currDistSqr) {
			// vector from bubble1 to 2
			ofVec2f dist = bubble2.pos - bubble1.pos;
			dist.normalize();
			dist *= sqrt(minDistSqr);
			bubble2.pos = bubble1.pos + dist;
		}
		
	}
	void update() {
		for(int i = 0; i < bubbles.size(); i++) bubbles[i].update();
		
		// check for collisions

		for(int i = 0; i < bubbles.size(); i++) {
			for(int j = 0; j < bubbles.size(); j++) {
				if(i!=j) {
					collision(bubbles[j], bubbles[i]);
				}
			}
		}
		// check if bubbles have disappeared off the edge
		// (This doesn't work perfectly.)
		for(int i = 0; i < bubbles.size(); i++) {
			if(bubbles[i].pos.x+bubbles[i].radius<0
			   ||
			   bubbles[i].pos.y+bubbles[i].radius<0
			   ||
			   bubbles[i].pos.x-bubbles[i].radius>getWidth()
			   ||
			   bubbles[i].pos.y-bubbles[i].radius>getHeight()) {
				
				bubbles.erase(bubbles.begin()+i);
				i--;
			}
		}
	}
	
	void draw() {
		//ofClear(100, 0, 0, 255);
		glDisable(GL_DEPTH_TEST);
		for(int i = 0; i < bubbles.size(); i++) bubbles[i].draw();
	}
	vector<Bubble> bubbles;
};