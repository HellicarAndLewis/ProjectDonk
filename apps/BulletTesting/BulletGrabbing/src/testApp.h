
#pragma once
#include "ofMain.h"
#include "ofxBullet.h"
#include "GL_ShapeDrawer.h"


class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	void drawViaBulletGL();
	void drawViaCustomGL();
	void drawSphere(const ofVec3f &center, float radius, int segments);
	
	bool bApplyForce;
	ofLight light;
	ofCamera camera;
	ofImage	 texture;
	
	ofxBullet bullet;
	GL_ShapeDrawer drawer;
};
