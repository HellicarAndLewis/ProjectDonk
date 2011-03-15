#include "testApp.h"

//ofShader shader;
ofVideoGrabber video;
//--------------------------------------------------------------
void testApp::setup(){
	mask.setup("mask.xml");
	mask.setEnabled(true);
	video.initGrabber(640, 480);
	//shader.setup("shader/blend.vert", "shader/blend.frag", "");
	
}

//--------------------------------------------------------------
void testApp::update(){
	video.update();
	mask.setTexture(&video.getTextureReference());
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	//shader.setUniformTexture("tex", video.getTextureReference(), 0);
	//shader.begin();
	//mask.draw();
	//shader.end();
//	ofEventArgs m;
//	mask.draw(m);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' ') mask.toggleEditing();
	if(key=='c') mask.clear();
	if(key=='z') mask.luminance+=0.2;
	if(key=='x') mask.luminance-=0.2;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	mask.gamma = ofMap(x, 0, ofGetWidth(), 0.5, 2.5);
	mask.blendPower = ofMap(y, 0, ofGetHeight(), 0.5, 5);
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

