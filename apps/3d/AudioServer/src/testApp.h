#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSoundStream.h"
#include "ofxXmlGui.h"
#include "ofxOsc.h"
#include "Sample.h"
class testApp : public ofBaseApp, public GuiListener {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void controlChanged(GuiControl *control);
	void audioReceived (float *buffer, int bufferSize, int nChannels);
	void audioRequested (float *buffer, int bufferSize, int nChannels);
	ofxSoundStream *soundStream;
	ofxXmlGui gui;
	float fps;
	ofxOscSender osc;
	bool customAudio;
	
	
	
	// locking stuff
	ofMutex audioMutex;
	float audioFps;
	float audioPos;
	float lastTimeSent;
	float smoothing;
	float exponent;
	
	Sample testSamples[6];
};

#endif	

