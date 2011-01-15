#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	img.loadImage("tdf_1972_poster.jpg");

	// get a reference from the original
	pixRef = img.getPixelsReference();

	// get a copy from the original
	pixCopy = img.getPixelsCopy();

	texRef.allocate(pixRef.getWidth(),pixRef.getHeight(),pixRef.getGlDataType());
	texCopy.allocate(pixRef.getWidth(),pixRef.getHeight(),pixRef.getGlDataType());
}

//--------------------------------------------------------------
void testApp::update(){
	// change pixels to red in the reference, the original changes
	pixRef[ofGetFrameNum()]=ofGetFrameNum()%3==0?255:0;

	// change pixels in the copy, the original doesn't change
	pixCopy[ofGetFrameNum()]=ofGetFrameNum()%2==0?255:0;

	// update the originals texture
	img.update();

	texRef.loadData(pixRef);
}

//--------------------------------------------------------------
void testApp::draw(){
	// draw the original
	img.draw(20,20);

	// draw the reference texture
	texRef.draw(40+img.getWidth(),20);

	// pass a ofPixels to a function by copy, no copy of the whole data
	// cause internally is a reference
	drawImage(pixCopy);

}

void testApp::drawImage(ofPixels pix){
	texCopy.loadData(pix);
	texCopy.draw(20,40 + img.getHeight());
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

