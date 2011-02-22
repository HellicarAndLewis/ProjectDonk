/*
 *  SphereReflection2D.cpp
 *  emptyExample
 *
 *  Created by Josh Nimoy on 2/19/11.
 *
 */

#include "SphereReflection2D.h"

namespace SphereReflection2D{

	int Sphere::reflection_resolution = 30;
	bool Sphere::draw_back_reflections = true;

	
	Sphere::Sphere(ofVec3f p,float r,ofColor c){
		pos = p;
		radius = r;
		color = c;
	}
	
	ofVec2f Sphere::spherize(ofVec2f p,double radius){
		double xx = p.x;
		double yy = p.y;
		double dist = sqrt(pow(xx,2.0)+pow(yy,2.0));
		if(dist<radius){
			xx *= (1 - dist/(radius*2))*2;
			yy *= (1 - dist/(radius*2))*2;
		}else{
			//nudge the point to the perimeter
			xx -= xx*(dist-radius)/dist;
			yy -= yy*(dist-radius)/dist;
		}	
		return ofVec2f(xx,yy);
	}

	void Sphere::drawReflection(ofVec3f targetPos, float targetRad, float zOff=0.1f){
		
		glBegin(GL_TRIANGLE_FAN);
		if(targetPos.z > 0 || draw_back_reflections){
			float theta = atan2(targetPos.y - pos.y,targetPos.x - pos.x);
			float r2d = sqrt(pow(targetPos.x - pos.x,2.0f)+pow(targetPos.y - pos.y,2.0f));
			float r3d = targetPos.distance(pos);
			float inc = TWO_PI/reflection_resolution;
			for(int i=0;i<reflection_resolution;i++){
				ofVec2f seg(r2d * cos(theta), r2d * sin(theta));
				float rad = targetRad*(targetRad/r3d);
				seg.x += rad * cos(inc*i);
				seg.y += rad * sin(inc*i);
				ofVec2f sp = spherize(seg,radius);
				glVertex3f(sp.x,sp.y,zOff);
			}
		}
		glEnd();
	}

}

