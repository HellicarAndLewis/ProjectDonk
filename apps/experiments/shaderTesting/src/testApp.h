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

#endif
