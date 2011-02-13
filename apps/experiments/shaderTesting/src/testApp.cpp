#include "testApp.h"

//cube mapping tutorial found here:
//http://developer.nvidia.com/object/cube_map_ogl_tutorial.html

//--------------------------------------------------------------
void testApp::setup(){
	

	
	bubbleShader.setup("fresnel_refraction.vs", "fresnel_refraction.fs");
	
	cubeMap.loadImages("skybox/berkeley_positive_x.png",
					   "skybox/berkeley_positive_y.png",
					   "skybox/berkeley_positive_z.png",
					   "skybox/berkeley_negative_x.png",
					   "skybox/berkeley_negative_y.png",
					   "skybox/berkeley_negative_z.png");
	
	glossMap.loadImage("white.png");
	baseMap.loadImage("Permutation Texture.png");
	
	bubbleShader.begin();
	bubbleShader.setUniform3f("fresnelValues", 0.1, 0, 0);
	bubbleShader.setUniform3f("IoR_Values", 0.5, 0.5, 0.5);
	
	//use multi-tex coords
	bubbleShader.setUniform1i("environmentMap", 0);
	//bubbleShader.setUniform1f("glossMap", 1);
	//bubbleShader.setUniform1f("baseMap", 2);
	
	// the amount that we're going to fade at the edges
	bubbleShader.setUniform1f("EdgeFalloff", 0.2);
	

	bubbleShader.end();
	
	//uniform float opacity;
	/*uniform samplerCube environmentMap;
	uniform sampler2D glossMap;
	uniform sampler2D baseMap;*/

	sphereCenter = ofVec3f(0, 0, 0);
	cam.setTarget(sphereCenter);
	cam.setDistance(300);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	
	cam.begin();
	
	bubbleShader.begin();
	
	cubeMap.bindMulti(0);
	
	ofSphere(sphereCenter, 30);
		
	cubeMap.unbind();	
	
	bubbleShader.end();
	
	cam.end();
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

