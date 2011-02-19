#include "testApp.h"
#include "SphereReflection2D.h"
#include <GLUT/GLUT.h>
using namespace SphereReflection2D;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(100,100,100);

	deque<Sphere> spheres;
	
	float tc_r = 100+cos(ofGetFrameNum()*0.09)*40;
	float tc_theta = ofGetFrameNum()*0.01;
	spheres.push_back(Sphere(ofVec3f(tc_r*cos(tc_theta),0,tc_r*sin(tc_theta)),
							 30,ofColor(255,255,255,128)));
	
	tc_r = 100+cos(ofGetFrameNum()*0.05)*40;
	tc_theta = ofGetFrameNum()*0.01 + PI;
	spheres.push_back(Sphere(ofVec3f(tc_r*cos(tc_theta),0,tc_r*sin(tc_theta)),
							 40,ofColor(255,255,255,128)));
	
	tc_r = 50+cos(ofGetFrameNum()*0.1)*40;
	tc_theta = ofGetFrameNum()*0.01 + PI/2;
	spheres.push_back(Sphere(ofVec3f(0,tc_r*cos(tc_theta),tc_r*sin(tc_theta)),
							 50,ofColor(255,255,255,128)));
	
	
	glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);	
	
	for(int i=0;i<spheres.size();i++){
		glPushMatrix();
		ofTranslate(spheres[i].pos);
		ofSetColor(spheres[i].color);
		ofCircle(0,0,spheres[i].radius);
		for(int j=0;j<spheres.size();j++){
			if(j!=i){
				ofSetColor(spheres[j].color);
				spheres[i].drawReflection(spheres[j].pos, spheres[j].radius, 0.2);
			}
		}
		glPopMatrix();
	}
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

