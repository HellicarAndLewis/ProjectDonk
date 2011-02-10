/**
 * AudioServerReceiver.h
 * AudioServerReceiver
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */
#include "ofxOsc.h"

class AudioServerReceiver: public ofBaseDraws {
public:

	AudioServerReceiver();
	void connect(int port = 12345);
	void update();
	int getNumChannels();
	float getVolume(int i);
	
	
	// for ofBaseDraws
	void draw(float x,float y,float w, float h);
	void draw(float x,float y) { draw(x, y, getWidth(), getHeight()); }
	float getHeight() { return 200; }
	float getWidth() { return 300; }
	
private:
	ofxOscReceiver osc;
	bool connected;
	int numChannels;
	float *signals;
	
};
