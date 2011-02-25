#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxJSON.h"
#include "App.h"
#include "Mode.h"
#include "BubbleData.h"
#include <vector>
#include "BubbleProjection.h"
#include "ofxTuioClient.h"
#include "CalibrationProjection.h"



class testApp : public Donk::App {

public:
	void setup();
	void setupGraphics();
	
	void update();
	
	void render();
	void drawView();
	

	void keyPressed  (int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	void touchUp(ofTouchEventArgs &touch);
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	
private:
	/**
	 osc object for receiving osc messages
	 */
	ofxOscReceiver oscIn;
	void setupOsc();
	void processOsc();

	void setupKinect();
	
	/**
	 json settings file
	 */
	ofxJSON json_settings;
	
	/** The mode we're going in to next - told to us by OSC /control/mode/next */
	string nextMode;
	
	Donk::Mode *mode;
	
	std::vector<Donk::BubbleData*> bubbles;
	
	BubbleProjection projection;
	CalibrationProjection calibrationProjection;
	ofxTuioClient tuio;
};

#endif
