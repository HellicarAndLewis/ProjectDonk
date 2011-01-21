#include "testApp.h"
#include "Plane.h"
#include "ofxXmlGui.h"

Plane plane;
ofxXmlGui gui;
//--------------------------------------------------------------
void testApp::setup(){
	gui.setup();
	gui.addSlider("A", plane.A, -1, 1);
	gui.addSlider("B", plane.B, -1, 1);
	gui.addSlider("C", plane.C, -1, 1);
	gui.addSlider("D", plane.D, -1, 1);
	gui.enable();
}

//--------------------------------------------------------------
void testApp::update(){

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

