#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
/*

poll URLs on a set increment
download their text
parse as json
interpret commands and act accordingly
 
 */
	ofSetVerticalSync(true);
	tcpClient.setVerbose(true);
	lastConnectTime = 0;
}

//--------------------------------------------------------------
void testApp::update(){
	
	if(tcpClient.isConnected()){
		string rcv = tcpClient.receiveRaw();
		cout << rcv;
		
	}else if(ofGetElapsedTimef() - lastConnectTime > 2 || lastConnectTime == 0){
		
		// not connected, but waited long enough,
		// or it's the first time, so calling right away
		
		if(!tcpClient.setup("173.236.165.127", 80, true)){
			cerr << "Error: failed to setup TCP client" << endl;
		}
		
		if(!tcpClient.sendRaw("GET / HTTP 1.0\n\n")){
			cerr << "Error: failed to send HTTP GET to web server" << endl;
		}
		
		lastConnectTime = ofGetElapsedTimef();
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

