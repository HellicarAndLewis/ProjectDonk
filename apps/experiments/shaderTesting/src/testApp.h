#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxCubeMap.h"
#include <GLUT/glut.h>

class testApp : public ofBaseApp{

	public:
	
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofShader cubeshader;
	ofImage glossMap, baseMap;
	
	ofxCubeMap cubeMap;
	
	ofVec3f sphereCenter;
	ofEasyCam cam;
	
	ofImage permTex;
	ofImage glossTex;
	
	bool hasDrawnTex;
	
	ofFbo first;
	ofFbo second;


};

#endif
