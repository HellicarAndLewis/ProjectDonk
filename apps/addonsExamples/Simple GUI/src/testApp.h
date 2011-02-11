#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofAddons.h"
#include "ofxSimpleGui.h"


class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		// The GUI
		ofxSimpleGui		GUI;
		
		// Things to control
		bool				doRanAmp;
		float				radius;
		float				amplitude;
		float				theta;
		int					r, g, b;
		bool				doTail;
		int					maxPoints;
		ofPoint*			tailPoint;
		
		ofPoint				pos;
		
		
};

#endif
	
