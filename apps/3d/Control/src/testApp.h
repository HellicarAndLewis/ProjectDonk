#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxNetwork.h"


/**
 
	implementation of non-blocking JSON loading from URLs and parsed out into OSC calls.
 
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
	ofxJSON settings;
	
	/**
		my ofxNetwork instance
	 */
	ofxTCPClient tcpClient;
	
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
		resolves a domain name to an IP address
	 */
	string getIpFromDomain(string name);

	/**
		logs to window console
	 */
	void log(string s);

	/**
		manages the HTTP protocol (separates header chunk from data chunk)
	 */
	void parseReceivedBytes(string incoming);
	
	/**
		whether or not the connection is waiting for header to finish
	 */
	bool http_header_mode;
	
	/**
		catching the returned "web page"
	 */
	string http_data_buffer;
	
	/**
		whether or not we expect the socket to be open
	 */
	bool http_expect_connected;
	
	/**
		parse the JSON into commands in preparation for OSC broadcasting
	 */
	void parseJSON(string &data);
	
	double polling_delay;
	
};

#endif
