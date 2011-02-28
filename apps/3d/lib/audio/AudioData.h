/**
 * AudioServerReceiver.h
 * AudioServerReceiver
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#define NUM_AUDIO_CHANNELS 6
namespace Donk {
class AudioData: public ofBaseDraws {
public:

	static AudioData *getInstance();
	
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
	AudioData();
	ofxOscReceiver osc;
	bool connected;
	int numChannels;
	float *signals;
	
};
}