#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	audioServer.connect();
	
	ofBackground( 30, 30, 130 );

}

//--------------------------------------------------------------
void testApp::update(){
	audioServer.update();
}


//--------------------------------------------------------------
void testApp::draw(){

	audioServer.draw(100, 100);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

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

