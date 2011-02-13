#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	
	bubbleShader.setup("fresnel_refraction.vs", "fresnel_refraction.fs");
	
	bubbleShader.setUniform3f("fresnelValues", 0.1, 0, 0);
	bubbleShader.setUniform3f("IoR_Values", 0.5, 0.5, 0.5);

	// the amount that we're going to fade at the edges
	bubbleShader.setUniform1f("EdgeFalloff", 0.2);
	
	//uniform float opacity;
	/*uniform samplerCube environmentMap;
	uniform sampler2D glossMap;
	uniform sampler2D baseMap;*/
	
	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	
	bubbleShader.begin();
	
	ofSphere(100, 100, 1, 100);
	
	bubbleShader.end();
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

