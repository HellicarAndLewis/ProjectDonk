/*
 *  SphereReflection2D.h
 *  emptyExample
 *
 *  Created by Josh Nimoy on 2/19/11.
 *
 */

#pragma once

#include "ofMain.h"

namespace SphereReflection2D{
	class Sphere{
	public:
		Sphere(ofVec3f p,float r,ofColor c);
		ofVec3f pos;
		float radius;
		ofColor color;
		static int reflection_resolution;
		static bool draw_back_reflections;
		static ofVec2f spherize(ofVec2f p,double radius);
		void drawReflection(ofVec3f targetPos, float targetRad,float zOff);
	};
}