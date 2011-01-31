#include "testApp.h"
#include "stdio.h"



#define NUM_AUDIO_CHANNELS 2
float volumes[NUM_AUDIO_CHANNELS];
//--------------------------------------------------------------
void testApp::setup(){	 
	
	ofBackground(255,255,255);	
	
	ofSoundStreamSetup(0,NUM_AUDIO_CHANNELS,this, 44100, 256, 1);	

}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	float width = (float)ofGetWidth()/(float)NUM_AUDIO_CHANNELS;

	for(int i = 0; i < NUM_AUDIO_CHANNELS; i++) {
		
		float y =(1.f-volumes[i])*ofGetHeight();
		
		ofSetHexColor(0xAA0000);
		ofFill();
		ofRect(i*width, y, width, ofGetHeight()-y);
		
		ofSetHexColor(0);
		ofNoFill();
		ofRect(i*width, y, width, ofGetHeight()-y);
		ofSetHexColor(0);
		
		ofDrawBitmapString(ofToString(volumes[i]), width*i+10, ofGetHeight() - 20);
	}
}


//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	


	
	for(int channel = 0; channel < nChannels; channel++) {
		for(int i = 0; i < bufferSize; i++) {

			float absSignal = ABS(input[i*nChannels+channel]);
			if(absSignal>volumes[channel]) {
				volumes[channel] = absSignal;
			} else {
				volumes[channel] *= 0.99995;
			}
		}
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}



