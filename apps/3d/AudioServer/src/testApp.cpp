#include "testApp.h"
#include "stdio.h"


#define NUM_AUDIO_CHANNELS 6
float volumes[NUM_AUDIO_CHANNELS];
float meters[NUM_AUDIO_CHANNELS];
float gains[NUM_AUDIO_CHANNELS];
float thruBuffer[NUM_AUDIO_CHANNELS][4096];
bool oscIsSetup;
bool mustStopAudio;

bool testing = false;
bool playThruAudio = false;
void testApp::exit() {
	printf("Exited\n");
	soundStream->close();
	
	

}
//--------------------------------------------------------------
void testApp::setup(){
	
	oscIsSetup = false;
	lastTimeSent = 0;
	ofSetWindowTitle("Donk Audio Server");
	ofLogNotice() << "Starting audio server";
	
	//ofLogNotice() << "Num channels: " << devices;
	ofBackground(255,255,255);	
	
	

	gui.loadFromXml("gui.xml");
	gui.enable();
	
	gui.pointToValue("updateFPS", &fps);
	for(int i = 0; i < NUM_AUDIO_CHANNELS; i++) {
		meters[i] = 0;
		volumes[i] = 0;
		gui.pointToValue("gain"+ofToString(i+1), &gains[i]);
		gui.pointToValue("ch"+ofToString(i+1), &meters[i]);
	}
	gui.pointToValue("dummy audio", &testing);
	gui.pointToValue("audio thru", &playThruAudio);
	gui.enableAutoSave("settings.xml");
	gui.addListener(this);
	ofSetWindowShape(gui.width, gui.height);

	
	testSamples[0].load("testaudio/bassdrum.wav");
	testSamples[1].load("testaudio/snare.wav");
	testSamples[2].load("testaudio/bass.wav");
	testSamples[3].load("testaudio/guitar.wav");
	testSamples[4].load("testaudio/organ.wav");
	testSamples[5].load("testaudio/vocals.wav");
	
	osc.setup(gui.getControlById("oscHost")->stringValue(), gui.getControlById("oscPort")->intValue());
	oscIsSetup = true;
	soundStream = new ofxSoundStream();
	soundStream->listDevices();
	if(soundStream->setDeviceIdByName("M-Audio, Inc.: M-Audio Fast Track Ultra")) {
		soundStream->setup(8, 8, this, 44100, 256, 1);
	   customAudio = true;
	} else {
		customAudio = false;
		ofSoundStreamSetup(2, 2, this, 44100, 256, 1);
//		soundStream.setup(0, 2, this, 44100, 256, 1);
	}
	
}

void testApp::update() {
	int fpsI = fps;
	ival(gui.getControlById("fps")->value) = fpsI;
	//for(int i = 0; i < NUM_AUDIO_CHANNELS; i++) {
	//	volumes[i] = gains[i]*ABS(sin(ofGetElapsedTimef()+i));
	//}
	audioMutex.lock();
	audioFps = fps;
	smoothing = 0.9999 + gui.getControlById("smoothing")->floatValue()*0.0001;
	exponent = pow(2, gui.getControlById("exponent")->floatValue());
	audioMutex.unlock();
}
//--------------------------------------------------------------
void testApp::draw(){
	
}

//--------------------------------------------------------------
void testApp::audioReceived(float *buffer, int bufferSize, int nChannels) {
	
	// clear out the thru buffer because we don't know if we're
	// going to use every channel yet.
	for(int i = 0; i < NUM_AUDIO_CHANNELS; i++) {
		memset(thruBuffer[i], 0, 4096*sizeof(float));
	}
	
	if(testing) nChannels = NUM_AUDIO_CHANNELS;

	for(int channel = 0; channel < NUM_AUDIO_CHANNELS && channel < nChannels; channel++) {
		for(int i = 0; i < bufferSize; i++) {
			float absSignal;
			if(testing) {
				float s = testSamples[channel].getSample();
				absSignal = ABS(s);
				thruBuffer[channel][i] = s*gains[channel];
			} else {
				absSignal = ABS(buffer[i*nChannels+channel]);
				thruBuffer[channel][i] = buffer[i*nChannels+channel]*gains[channel];
			}
			if(absSignal>volumes[channel]) {
				volumes[channel] = absSignal;
			} else {
				volumes[channel] *= smoothing;
			}
		}
		// clip 
		meters[channel] = MIN(volumes[channel]*gains[channel], 1);
		meters[channel] = pow(meters[channel], exponent);
	}
	
	// how long in seconds we've gone forward in time
	float frameAdvance = ((float)bufferSize/44100.f);
	audioPos += frameAdvance;
	
	audioMutex.lock();
	float framePeriod = 1.f/audioFps;
	
	audioMutex.unlock();
	
	if(audioPos - lastTimeSent>framePeriod) {
		lastTimeSent = audioPos;

		if(oscIsSetup) {
			ofxOscMessage m;
			m.setAddress("/audio");
			for(int i = 0; i < NUM_AUDIO_CHANNELS; i++) {
				m.addFloatArg(pow(MIN(volumes[i]*gains[i], 1), exponent));
			}	
		
			osc.sendMessage(m);
		}
	}
	
}

void testApp::audioRequested (float *buffer, int bufferSize, int nChannels) {
	if(playThruAudio) {
		// just use the first 2 channels
		for(int i = 0; i < bufferSize; i++) {
			buffer[i*2] = (
						   thruBuffer[0][i]
						   +thruBuffer[1][i]
						   +thruBuffer[2][i]
						   +thruBuffer[3][i]
						   +thruBuffer[4][i]
						   +thruBuffer[5][i]) / 8.f;
			buffer[i*2+1] = buffer[i*2];
		}
	} else {
		memset(buffer, 0, bufferSize*nChannels*sizeof(float));
	}
}
void testApp::controlChanged(GuiControl *control) {
	if(control->controlId=="connect") {
		// reconnect osc
		osc.setup(gui.getControlById("oscHost")->stringValue(), gui.getControlById("oscPort")->intValue());
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



