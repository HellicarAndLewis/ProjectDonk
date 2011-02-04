/**
 * ofxPlanarKinect.cpp
 * KinectPlanarExperiment
 *
 * Created by Marek Bereza on 04/02/2011.
 *
 */

#include "ofxXmlSettings.h"

// downsample factor of depth graph
#define ofxPlanarKinect_depthGraphResolution 2

#include "ofxPlanarKinect.h"

ofxPlanarKinect::ofxPlanarKinect() {
	pixels = NULL;
	depthGraph = NULL;
	mouseIsDown = false;
	width = 640;
	height = 480;
	threshold = 128;
	// clockwise from top left
	outputQuad[0] = ofVec2f(0,0);
	outputQuad[1] = ofVec2f(1,0);
	outputQuad[2] = ofVec2f(1,1);
	outputQuad[3] = ofVec2f(0,1);
}

ofxPlanarKinect::~ofxPlanarKinect() {
	if(pixels!=NULL) {
		delete [] pixels;
	}
	if(depthGraph!=NULL) {
		delete [] depthGraph;
	}
	
}

void ofxPlanarKinect::setup() {

	dims = ofVec2f(640, 480); // or whatever the kinect is.
	sliceY = dims.y/2;
	camImg.allocate(dims.x, dims.y, GL_LUMINANCE);
	sliceImg.allocate(dims.x, 1, GL_LUMINANCE);
	numPixels = dims.x*dims.y;
	pixels = new unsigned char[numPixels];
	numDepthGraphPoints = dims.x/ofxPlanarKinect_depthGraphResolution;
	depthGraph = new ofVec2f[numDepthGraphPoints];
	loadSettings();	
}


void ofxPlanarKinect::update(unsigned char *pixels) {
	if(pixels==NULL) {
		ofLog(OF_LOG_ERROR, "ofxPlanarKinect(): setup() not called, or set up incorrectly!");
	}
	
	// copy the whole frame
	memcpy(this->pixels, pixels, numPixels);
	
	// then make a reference to the slice
	int offset = sliceY*dims.x;
	slice = this->pixels + offset;
	
	// load the preview texture
	camImg.loadData(this->pixels, dims.x, dims.y, GL_LUMINANCE);
	
	// do the computer vision stuff
	preprocessSlice();
	findBlobs();
	calibrateBlobs();
	
	// load the slice texture with a preprocessed slice array
	sliceImg.loadData(slice, dims.x, 1, GL_LUMINANCE);
	
	// calculate the depth graph points (for preview)
	int pos = 0;
	float scale = 1*width/dims.x;
	for(int i = 0; i < dims.x; i+=ofxPlanarKinect_depthGraphResolution) {
		depthGraph[pos] = ofVec2f(x + i*scale, y+ofMap(slice[i], 0, 255, 0, height));
		pos++;
	}
}


void ofxPlanarKinect::draw() {
	draw(x, y, width, height);
}

void ofxPlanarKinect::draw(float x,float y) {
	draw(x, y, getWidth(), getHeight());
}



// these return the dims of the actual drawable, not the kinect itself
float ofxPlanarKinect::getHeight() {
	return height;
}
float ofxPlanarKinect::getWidth() {
	return width;
}

void ofxPlanarKinect::draw(float x,float y,float w, float h) {
	this->x = x;
	this->y = y;
	width = w;
	height = h;
	
	ofSetHexColor(0xFFFFFF);
	if(mouseIsDown && lastButton==0) {
		camImg.draw(x,y,width,height);
		
		ofSetHexColor(0xFF0000);
		ofNoFill();
		ofRect(x, y + (sliceY-1)*height/dims.y, width, 3);
		ofFill();
	} else {
		ofSetHexColor(0xFFFFFF);
		sliceImg.draw(x,y,width,height);
		ofSetHexColor(0xFF0000);
		ofDrawBitmapString("Click mouse to choose a slice,\nright click to set threshold", x+3,y+14);
		glBegin(GL_LINE_STRIP);
		for(int i = 0 ; i < numDepthGraphPoints; i++)
			glVertex2f(depthGraph[i].x, depthGraph[i].y);
		glEnd();
		ofSetHexColor(0x00FF00);
		float t = ofMap(threshold, 0, 255, y, y+height);
		ofLine(x, t, x+width, t);
		for(int i = 0; i < rawBlobs.size(); i++) {
			ofCircle(
							x + ofMap(rawBlobs[i].x, 0, dims.x, 0, width),
							y + ofMap(rawBlobs[i].y, 0, 255, 0, height), 7);
		}
	}
}


void ofxPlanarKinect::mousePressed(float x, float y, int button) {
	mouseIsDown = inside(x,y);
	if(mouseIsDown) {
		if(button==0) {
			sliceY = (y - this->y)*dims.y/height;
		} else {
			threshold = (y - this->y)*255.f/height;
		}
	}
	lastButton = button;
}

void ofxPlanarKinect::mouseReleased(float x, float y, int button) {
	mouseIsDown = false;
	saveSettings();
}

void ofxPlanarKinect::mouseDragged(float x, float y, int button) {
	mousePressed(x, y, button);
}
	
void ofxPlanarKinect::preprocessSlice() {
	// start with at least the first pixel being a lowpass filtered value of all the pixels that are not 0
	if(slice[0]==0) {
		float val = 0;
		for(int i = 1; i < dims.x; i++) {
			if(slice[i]!=0) {
				if(val==0) {
					val = slice[i];
				} else {
					val = val*0.92 + ((float)slice[i])*0.08;
				}
			}
		}
		slice[0] = val;
		if(val>255) printf("First slice pixel too big! %f\n", val);
	}
	
	// flood fill any black out with previous pixels
	for(int i = 1; i < dims.x; i++) {
		if(slice[i]==0) slice[i] = slice[i-1];
	}
	
	
}

void ofxPlanarKinect::findBlobs() {
	
	rawBlobs.clear();
	
	// look through slice and decide where the blobs are.
	
	for(int i = 0; i < dims.x; i++) {
		// loop until we find a blob
		while(i<dims.x && slice[i]<threshold) {
			i++;
		}

		// consume the blob
		if(slice[i]>=threshold) {
			int blobStart = i;
			// find the end point (and also the depth)
			float maxDepth = slice[i];
			int blobCentre = 0;
			while(i<dims.x && slice[i]>threshold) {
				if(slice[i]>maxDepth) {
					maxDepth = slice[i];
					blobCentre = i;
				}
				i++;
			}
			
			int blobEnd = i;
			
			// give the centre of the blob as x-coord
			blobCentre = (blobEnd+blobStart)/2;
			
			rawBlobs.push_back(ofVec2f(blobCentre, maxDepth));
		}
	}
	
}

void ofxPlanarKinect::calibrateBlobs() {
	blobs.clear();
	
	// first step is to set up the warper matrix. You do this by giving the coordinates
	// of the desired screen rectangle in kinect coords as the source and the values
	// you want them to map to. A bit like ofMap but in 2d.
	
	// these are the raw coordinates for the corners of the screen rectangle
	// the input to the warper goes top-left, top-right, bottom-left, bottom-right
	warper.setSource(inputQuad[0].x, inputQuad[0].y,
					 inputQuad[1].x, inputQuad[1].y,
					 inputQuad[3].x, inputQuad[3].y,
					 inputQuad[2].x, inputQuad[2].y);
	
	// these should be just normalized screen coordinates (0-1)
	warper.setDestination(outputQuad[0].x, outputQuad[0].y,
						  outputQuad[1].x, outputQuad[1].y,
						  outputQuad[3].x, outputQuad[3].y,
						  outputQuad[2].x, outputQuad[2].y);
	
	
	// this calculates the transformation matrices.
	warper.computeWarp();
	
	// iterate through each blob and run it through the matrix.
	for(int i = 0; i < rawBlobs.size(); i++) {
		ofVec2f out;
		warper.warp(rawBlobs[i].x, rawBlobs[i].y, &out.x, &out.y);
		blobs.push_back(out);
	}
}

void ofxPlanarKinect::keyPressed(int key) {
	
	// clockwise from top left
	if(key=='1') {
		if(rawBlobs.size()>0) inputQuad[0] = rawBlobs[0];
	} else if(key=='2') {
		if(rawBlobs.size()>0) inputQuad[1] = rawBlobs[0];
	} else if(key=='3') {
		if(rawBlobs.size()>0) inputQuad[2] = rawBlobs[0];
	} else if(key=='4') {
		if(rawBlobs.size()>0) inputQuad[3] = rawBlobs[0];
	}
	saveSettings();
}

void ofxPlanarKinect::saveSettings() {
	ofxXmlSettings xml;
	xml.addTag("settings");
	xml.addAttribute("settings", "x0", inputQuad[0].x, 0);
	xml.addAttribute("settings", "y0", inputQuad[0].y, 0);
	xml.addAttribute("settings", "x1", inputQuad[1].x, 0);
	xml.addAttribute("settings", "y1", inputQuad[1].y, 0);
	xml.addAttribute("settings", "x2", inputQuad[2].x, 0);
	xml.addAttribute("settings", "y2", inputQuad[2].y, 0);
	xml.addAttribute("settings", "x3", inputQuad[3].x, 0);
	xml.addAttribute("settings", "y3", inputQuad[3].y, 0);
	xml.addAttribute("settings", "threshold", threshold, 0);
	xml.addAttribute("settings", "sliceY", sliceY, 0);
	xml.saveFile("planarKinectSettings.xml");
}

void ofxPlanarKinect::loadSettings() {
	ofxXmlSettings xml;
	if(xml.loadFile("planarKinectSettings.xml")) {

		inputQuad[0].x = xml.getAttribute("settings", "x0", inputQuad[0].x);
		inputQuad[0].y = xml.getAttribute("settings", "y0", inputQuad[0].y);
		inputQuad[1].x = xml.getAttribute("settings", "x1", inputQuad[1].x);
		inputQuad[1].y = xml.getAttribute("settings", "y1", inputQuad[1].y);
		inputQuad[2].x = xml.getAttribute("settings", "x2", inputQuad[2].x);
		inputQuad[2].y = xml.getAttribute("settings", "y2", inputQuad[2].y);
		inputQuad[3].x = xml.getAttribute("settings", "x3", inputQuad[3].x);
		inputQuad[3].y = xml.getAttribute("settings", "y3", inputQuad[3].y);
		threshold      = xml.getAttribute("settings", "threshold", threshold);
		sliceY		   = xml.getAttribute("settings", "sliceY", sliceY);
		
		ofLog(OF_LOG_NOTICE, "Loaded planar kinect settings beautifully\n");
	} else {
		ofLog(OF_LOG_ERROR, "Could not load planar kinect settings. Maybe this is a first run?\n");
	}
}