/**
 * AudioServerReceiver.cpp
 * AudioServerReceiver
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */


#include "AudioData.h"
using namespace Donk;

AudioData::AudioData() {
	connected = false;
	numChannels = 6; // default value
	signals = NULL;
}

void AudioData::connect(int port) {
	osc.setup(port);
	connected = true;
}

void AudioData::update() {
	if(!connected) {
		ofLog(OF_LOG_WARNING, "OSC to AudioServer not connected, maybe you didn't call connect(port)?\n");
		return;
	}
	
	while(osc.hasWaitingMessages()) {
		ofxOscMessage m;
		osc.getNextMessage(&m);
		if(m.getAddress()=="/audio") {
			
			
			// if the number of channels has changed, re-allocate
			int newNumChannels = m.getNumArgs();
			if(newNumChannels!=numChannels && signals!=NULL) {
				delete [] signals;
				signals = NULL;
			}
			
			numChannels = newNumChannels;
			
			if(signals==NULL) {
				signals = new float[numChannels];
			}
			
			for(int i = 0; i < numChannels; i++) {
				signals[i] = m.getArgAsFloat(i);
//				cout << "Audio value, " << i << " is: " << m.getArgAsFloat(i) << endl;
			}
		}
	}
}

int AudioData::getNumChannels() {
	return numChannels;
}
float AudioData::getVolume(int i) {
	// check bounds
	if(signals==NULL) return 0;
	if(i<0 || i >= numChannels) return 0;
	return signals[i];
}

void AudioData::draw(float x,float y,float w, float h) {
	float width = (float)getWidth()/(float)getNumChannels();
	
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	ofFill();
	for(int i = 0; i < getNumChannels(); i++) {
		
		float y =(1.f-getVolume(i))*h;
		
		ofSetHexColor(0xAA0000);
		ofRect(i*width, y, width, h-y);
		
		ofSetHexColor(0);
		ofNoFill();
		ofRect(i*width, y, width, h-y);
		ofFill();
	}
	
	ofSetHexColor(0xFFFFFF);
	ofNoFill();
	ofRect(0, 0, w, h);
	ofFill();
	glPopMatrix();
}

AudioData *AudioData::getInstance() {
	static AudioData *instance = NULL;
	if(instance==NULL) instance = new AudioData();
	return instance;
}
