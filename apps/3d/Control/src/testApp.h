#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxOsc.h"
#include "AsyncHttpLoader.h"
#include "ControlBar.h"

/**
	asyncronous http gets to 4 urls, receiving JSON
	and sending it all to the render machine.
 */
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
	deque<string> console;
	/**
		console line colors
	 */
	deque<int> console_colors;
	
	/**
		font for console rendering
	 */
	ofTrueTypeFont font;

	/**
		logs to window console
	 */
	void log(string s,int color);
	void log(int n,int color);
	
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
		rendermachine osc port
	 */
	unsigned long rendermachine_osc_port;
	
	/**
		rendermachine IP address
	 */
	string rendermachine_ip;
	
	/**
		must be a pointer since it passes
		itself to a thread func
	 */
	vector<AsyncHttpLoader*> loaders;
	/**
		http get timeout in seconds
	 */
	int http_get_timeout;
	/**
		username:password from settings file
	 */
	string http_auth;
	/**
		gui -- sorry, ofxSimpleGui has no buttons.
	 */
	ControlBar::Bar controlbar;
	/**
		current and upcoming modes
		referring to ints
	 */
	int modes[2];
	
	/**
		populate osc message with json bubble data
	 */
	void populateBubble(ofxOscMessage &m,Json::Value &bubble);
	/**
		poll a URL by source index
	 */
	void poll(int index);
	/**
		updates the poll buttons to be enabled
		or disabled depending on the manual_poll
		settings for that source
	 */
	void updateEnabledButtons();
};

#endif
