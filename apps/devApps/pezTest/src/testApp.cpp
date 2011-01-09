#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	 
	ofBackground(255, 255, 255);
}


//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0, 0, 0);
	ofDrawBitmapString("Note: Key pressed doesn't currently work - so ESC / Apple Q isn't hooked up", 10, 20);
	ofDrawBitmapString("Drag images from the finder into my window!", ofGetWidth()/2 - 200,	ofGetHeight()/2);

	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255);
	for(int k = 0; k < img.size(); k++){
		img[k].draw(draggedPos.x + k*240, draggedPos.y + k*240);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){ 
	printf("KEY PRESSED %c\n", key);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	printf("testApp::mouseDragged %i %i \n", x, y);
}
 
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	printf("testApp::mousePressed %i %i \n", x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	printf("testApp::mouseReleased %i %i \n", x, y);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::filesDragged(vector <string> filePaths, ofPoint dragPos){
	draggedPos = dragPos;

	img.clear();
	img.assign(filePaths.size(), ofImage());
	for(int k = 0; k < filePaths.size(); k++){
		printf("[%i] = %s\n", k, filePaths[k].c_str());
		img[k].loadImage(filePaths[k]);
	}
}	
