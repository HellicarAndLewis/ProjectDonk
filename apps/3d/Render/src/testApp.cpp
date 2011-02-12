#include "testApp.h"
#include "BubbleData.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	if(!json_settings.loadFromFile("settings.json.txt")){
		cerr << "failed to load json settings file" << endl;
		_exit(1);
	}
	
	oscIn.setup(json_settings["osc_listen_port"].asInt());
}

//--------------------------------------------------------------
void testApp::update(){

	while(oscIn.hasWaitingMessages()){
		ofxOscMessage m;
		oscIn.getNextMessage(&m);
		if(m.getAddress()=="/control/mode/next"){
			
			//get the next mode ready
			string modeName = m.getArgAsString(0);
			
			cout << "todo: start upcoming mode standby for mode: " << modeName << endl;
			
		}else if(m.getAddress()=="/control/mode/swap"){
			
			cout << "todo: begin transition from current to upcoming mode." << endl;
			
		}else if(m.getAddress()=="/control/bubble/new"){
			Donk::BubbleData bd(m);
			cout << "new bubble received from " << bd.userName << " in mode " << bd.mode << endl;
			
			//todo: do something with the bubble data
			
		}
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
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
