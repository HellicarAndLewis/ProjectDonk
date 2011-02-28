#include "testApp.h"
#include "stdio.h"


#define NUM_AUDIO_CHANNELS 6
float volumes[NUM_AUDIO_CHANNELS];
float meters[NUM_AUDIO_CHANNELS];
float gains[NUM_AUDIO_CHANNELS];
bool oscIsSetup;
bool mustStopAudio;


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
	gui.enableAutoSave("settings.xml");
	gui.addListener(this);
	ofSetWindowShape(gui.width, gui.height);
	
	osc.setup(gui.getControlById("oscHost")->stringValue(), gui.getControlById("oscPort")->intValue());
	oscIsSetup = true;
	soundStream = new ofxSoundStream();
	soundStream->listDevices();
	if(soundStream->setDeviceIdByName("M-Audio, Inc.: M-Audio Fast Track Ultra")) {
		soundStream->setup(8, 8, this, 44100, 256, 1);
	   customAudio = true;
	} else {
		customAudio = false;
		ofSoundStreamSetup(0, 2, this, 44100, 256, 1);
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
	printf("Smoothing %f\n", smoothing);
	audioMutex.unlock();
}
//--------------------------------------------------------------
void testApp::draw(){
	
}


//--------------------------------------------------------------
void testApp::audioReceived(float *buffer, int bufferSize, int nChannels) {
	
	for(int channel = 0; channel < NUM_AUDIO_CHANNELS && channel < nChannels; channel++) {
		for(int i = 0; i < bufferSize; i++) {

			float absSignal = ABS(buffer[i*nChannels+channel]);
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



