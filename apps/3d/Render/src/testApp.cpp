#include "testApp.h"
#include "BubbleData.h"
#include "QuestionData.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	if(!json_settings.loadFromFile("settings/OSC.json.txt")){
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
			cout << "todo: do something with new bubble received from " << bd.userName << " in mode " << bd.mode << endl;
			
		}else if(m.getAddress()=="/control/question/update"){
			
			Donk::QuestionData qd(m);
			cout << "todo: handle question update \"" << qd.text << "\" " << qd.tags[0] << "=" <<
				qd.tag_counts[0] << "," << qd.tags[1] << "=" << qd.tag_counts[1] << endl;
			
		}else if(m.getAddress()=="/audio"){
			
			//get 6 floated values
			float audioData[6];
			for(int i=0;i<6;i++)audioData[i] = m.getArgAsFloat(i);
			
			cout << "todo: do something with new audio frequency sample: " <<
				audioData[0] << ' ' <<
				audioData[1] << ' ' <<
				audioData[2] << ' ' <<
				audioData[3] << ' ' <<
				audioData[4] << ' ' <<
				audioData[5] << endl;
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
