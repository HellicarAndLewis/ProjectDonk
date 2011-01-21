/*
 *  Plane.h
 *  audioReactive
 *
 *  Created by Marek Bereza on 21/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */
/**
 * Represents a plane in 3d space.
 * Uses the equation Ax + By + Cz + D = 0
 *
 * Lots of help from Mr Bourke: http://local.wasp.uwa.edu.au/~pbourke/geometry/planeeq/
 */
class Plane {
public:
	
	float A; // x-coefficient
	float B; // y-coefficient
	float C; // z-coefficient
	float D; // constant
	
	void setFrom3Points(ofVec3f p1, ofVec3f p2, ofVec3f p3) {
		
		
		
		
		A = p1.y * (p2.z - p3.z) + p2.y * (p3.z - p1.z) + p3.y * (p1.z - p2.z);
		B = p1.z * (p2.x - p3.x) + p2.z * (p3.x - p1.x) + p3.z * (p1.x - p2.x);
		C = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
		D = -(p1.x * (p2.y * p3.z - p3.y * p2.z) + p2.x * (p3.y * p1.z - p1.y * p3.z) + p3.x * (p1.y * p2.z - p2.y * p1.z));
	}
	
	Plane() {
		A = B = C = D = 0;
	}
};