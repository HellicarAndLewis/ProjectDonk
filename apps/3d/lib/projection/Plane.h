/*
 *  Plane.h
 *  audioReactive
 *
 *  Created by Marek Bereza on 21/01/2011.
 *
 */

#define PLANE_X 0
#define PLANE_Y 1
#define PLANE_Z 2

/**
 * \brief Represents a plane in 3d space, and helps with slicing images out of a kinect.
 * Uses the equation Ax + By + Cz + D = 0
 *
 * Lots of help from Mr Bourke: http://local.wasp.uwa.edu.au/~pbourke/geometry/planeeq/
 */
class Plane {
public:
	

	/**
	 * This calculates the plane equation from 3 points in space.
	 */
	void setFrom3Points(ofVec3f p1, ofVec3f p2, ofVec3f p3) {
		
		
		
		// calculate non-normalized coefficients of the plane equation.
		a = p1.y * (p2.z - p3.z) + p2.y * (p3.z - p1.z) + p3.y * (p1.z - p2.z);
		b = p1.z * (p2.x - p3.x) + p2.z * (p3.x - p1.x) + p3.z * (p1.x - p2.x);
		c = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
		d = -(p1.x * (p2.y * p3.z - p3.y * p2.z) + p2.x * (p3.y * p1.z - p1.y * p3.z) + p3.x * (p1.y * p2.z - p2.y * p1.z));
		
		// calculate normalized version
		float coeff = 1.f/sqrt(a*a + b*b + c*c);
		a *= coeff;
		b *= coeff;
		c *= coeff;
		d *= coeff;
	}
	
	Plane() {
		a = b = c = d = 0.5;
	}
	
	/**
	 * This gives you the value of a coord given the two others
	 * e.g. give it 
	 * solve(ofVec3f(0, 1, 1), PLANE_X)
	 * and it will calculate the x-coordinate, and put it into
	 * the point ref supplied in the parameter.
	 */
	void solve(ofPoint &p, int which) {
		if(which==PLANE_X) {
			// x  = - (By + Cz + D)/A
			p.x = -(b*p.y + c*p.z + d)/a;
		} else if(which==PLANE_Y) {
			// y = -(Ax + Cz + D)/B
			p.y = -(a*p.x + c*p.z + d)/b;
		} else if(which==PLANE_Z) {
			// z = - (Ax + By + D)/C
			p.z = -(a*p.x + b*p.y + d)/c;
		}
	}
	ofVec3f getNormal() {
		return ofVec3f(a, b, c);
	}
	
	
	
	
	
	
	float distanceToPoint(ofVec3f p) {
		return a*p.x + b*p.y + c*p.z + d;
	}
	
	/**
	 * This isn't perfect, but you probably don't need it.
	 */
	void draw() {
		ofPoint p[4];
		p[0] = ofPoint(-.5, 0.5);
		p[1] = ofPoint(.5, 0.5);
		p[2] = ofPoint(.5, -.5);
		p[3] = ofPoint(-.5, -.5);
		
		for(int i = 0; i < 4; i++) {
			solve(p[i], PLANE_Z);
			
		}
		glColor3f(1, 1, 1);
		drawQuad(p);
		
		// draw the centre of the plane
		glColor3f(0, 0, 1);
		ofVec3f normal = getNormal();
		ofLine(0, 0, 0, normal.x, normal.y, normal.z);
	}

private:
	float a; // x-coefficient
	float b; // y-coefficient
	float c; // z-coefficient
	float d; // constant
	
	void drawQuad(ofPoint *p) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &p[0]);
		glDrawArrays(GL_QUADS, 0, 4);
	}
};
