#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxOsc.h"

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

	/**
		my JSON parser instance
	 */
	ofxJSON json;
	
	/**
		another JSON instance just for the app settings
	 */
	ofxJSON json_settings;
		
	/**
		last time a server pull was done (in seconds)
	 */
	float lastConnectTime;
	
	/**
		window console buffer
	 */
	string console;
	
	/**
		font for console rendering
	 */
	ofTrueTypeFont font;

	/**
		logs to window console
	 */
	void log(string s);
	void log(int n);
	
	double polling_delay;
	
	/**
		category names for each json URL
	 */
	vector<string> source_names;
	
	/**
		osc object for sending osc messages
	 */
	ofxOscSender oscOut;
	
	/**
		osc port
	 */
	unsigned long osc_port;
	
	/**
		osc destination IP addresses
	 */
	vector<string> osc_destinations;
};

#endif
