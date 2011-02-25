#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "BrownianObject.h"

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
	
	vector<BrownianObject*> particles;
	
	void createObjectAtPoint(ofVec3f point);
	
	ofCamera cam;
	ofLight	light;
	bool drawSphere;
	
	vector<ofVec3f> disturbingObjects;
	
	ofVec3f emitter;
};
 

#endif
