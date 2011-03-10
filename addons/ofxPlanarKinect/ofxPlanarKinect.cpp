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

	inset = 0;
	// should really pass the kinect in here
	kinectWidth = 640;
	kinectHeight = 480;
	minHandWidth = 10;
	maxHandWidth = 640/4;
	pixels = NULL;
	depthGraph = NULL;
	width = 640;
	height = 480;
	cropLeft = 0;
	cropRight = 1;
	threshold = new float[(int)kinectWidth];
	for(int i = 0; i < kinectWidth; i++) {
		threshold[i] = 128;
	}
	deviceId = 0;
	guiMode = SLICE_SELECTION;
	
	// clockwise from top left
	outputQuad[0] = ofVec2f(0,0);
	outputQuad[1] = ofVec2f(1,0);
	outputQuad[2] = ofVec2f(1,1);
	outputQuad[3] = ofVec2f(0,1);
	lpf = 0.9;
	timeFilter = 0.5;
	fillHoles = false;
	lastSlice = new float[(int)kinectWidth];
	currFrame = new float[(int)(kinectHeight*kinectWidth)];
	interactionDepth = 20;
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


	
	// this is the default position of where we take the row of pixels from.
	sliceY = kinectHeight/2;
	
	// allocate gui textures
	camImg.allocate(kinectWidth, kinectHeight, GL_LUMINANCE);
	sliceImg.allocate(kinectWidth, 1, GL_LUMINANCE);
	
	numPixels = kinectWidth*kinectHeight;
	pixels = new unsigned char[numPixels];
	
	// this is for the vertex array of points in the depth graph
	numDepthGraphPoints = kinectWidth/ofxPlanarKinect_depthGraphResolution;
	depthGraph = new ofVec2f[numDepthGraphPoints];
	
	// load settings from a file if it exists.
	loadSettings();	
	sliceChooser.setup(&sliceY, &camImg, kinectHeight, &interactionDepth);
	thresholdControl.setup(kinectWidth, &blobs, &sliceImg, depthGraph, numDepthGraphPoints, &rawBlobs, threshold);
}



int ROTATE_PIXEL_COORD(int i, int W) {
	
	int x = i%W;
	int y = i/W;
	return x*W + y;
//	((i%W)*W + (i/W));
}

void ofxPlanarKinect::update(float *distances) {
	
	if(sliceY-interactionDepth<0) interactionDepth = sliceY-1;
	
	
	if(pixels==NULL) {
		ofLog(OF_LOG_ERROR, "ofxPlanarKinect(): setup() not called, or set up incorrectly!");
	}
	
	// make a reference to this for later if needed
	distancesRef = distances;
	
	int kw = kinectWidth;
	int cl = kinectWidth*cropLeft;
	int cr = kinectWidth*cropRight;
	
	// copy + map the whole frame
	for(int i = 0; i < numPixels; i++) {
		
		// do the crop at this stage
		if(i%kw<cl || i%kw>cr) 
			this->currFrame[i] = 0;
		else 
			this->currFrame[i] = ofMap(distances[i], 400, 0, 0, 1, true);
	}
		
	if(fillHoles) { // fill the holes for the whole frame
		for(int i = 1; i < numPixels; i++) {
			if(this->currFrame[i]==1) {
				int start = i-1;
				for(; i < numPixels; i++) {
					if(this->currFrame[i]!=1) {
						for(int j = start; j < i; j++) {
							this->currFrame[j] = ofMap(j, start, i, this->currFrame[start], this->currFrame[i]);
						}

						break;
					}
				}
			}
		}
	}
		
	// copy to the render frame.
	for(int i = 0; i < numPixels; i++) {
		this->pixels[i] = this->currFrame[i]*255.f;
	}
	
	// move the slice pointer to wherever sliceY is.
	slice = this->currFrame + ((int)kinectWidth*sliceY);

	// similarly, position the interactionArea
	interactionArea = this->currFrame + ((int)kinectWidth*(sliceY-interactionDepth));
	
	
	//slice = this->pixels + offset;
	
	// load the preview texture
	camImg.loadData(this->pixels, kinectWidth, kinectHeight, GL_LUMINANCE);
	
	// do the computer vision stuff
	preprocessSlice();
	findBlobs();
	calibrateBlobs();
	
	// make a temporary unsigned char representation of the slice
	unsigned char *slc = new unsigned char[(int)kinectWidth];
	
	for(int i = 0; i< kinectWidth; i++) {
		slc[i] = ofMap(slice[i], 0, 1, 0, 255, true);
	}
	// load the slice texture with a preprocessed slice array
	sliceImg.loadData(slc, kinectWidth, 1, GL_LUMINANCE);
	delete [] slc;
	
	// calculate the depth graph points (for preview)
	int pos = 0;
	float scale = 1*width/kinectWidth;
	for(int i = 0; i < kinectWidth; i+=ofxPlanarKinect_depthGraphResolution) {
		depthGraph[pos] = ofVec2f(i, slice[i]);
		pos++;
	}
		
}

void ofxPlanarKinect::moveThreshold(float increment) {
	for(int i = 0; i < kinectWidth; i++) {
		threshold[i] = ofClamp(threshold[i]+increment, 0, 1);
	}
}

void ofxPlanarKinect::captureThreshold() {
	for(int i = 0; i < kinectWidth; i++) {
		threshold[i] = ofClamp(slice[i]+0.02, 0, 1);
	}
}


void ofxPlanarKinect::calibrateCorner(int whichCorner) {	
	// clockwise from top left
	if(whichCorner==TOP_LEFT_CORNER) {
		if(rawBlobs.size()>0) inputQuad[0] = rawBlobs[0];
	} else if(whichCorner==TOP_RIGHT_CORNER) {
		if(rawBlobs.size()>0) inputQuad[1] = rawBlobs[0];
	} else if(whichCorner==BOTTOM_RIGHT_CORNER) {
		if(rawBlobs.size()>0) inputQuad[2] = rawBlobs[0];
	} else if(whichCorner==BOTTOM_LEFT_CORNER) {
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
	
	for(int i = 0; i < kinectWidth; i++) {
		xml.addAttribute("settings", "threshold"+ofToString(i), threshold[i], 0);
	}
	
	xml.addAttribute("settings", "sliceY", sliceY, 0);
	xml.addAttribute("settings", "deviceId", deviceId, 0);
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

		
		for(int i = 0; i < kinectWidth; i++) {
			threshold[i] = xml.getAttribute("settings", "threshold"+ofToString(i), threshold[i]);
//			printf("threshold %d = %f\n", i, threshold[i]);
		}
		sliceY		   = xml.getAttribute("settings", "sliceY", sliceY);
		deviceId	   = xml.getAttribute("settings", "deviceId", deviceId);
		ofLog(OF_LOG_NOTICE, "Loaded planar kinect settings beautifully\n");
	} else {
		ofLog(OF_LOG_ERROR, "Could not load planar kinect settings. Maybe this is a first run?\n");
	}
}

ofxPlanarKinectGuiElement &ofxPlanarKinect::getSliceChooser() {
	return sliceChooser;
}
ofxPlanarKinectGuiElement &ofxPlanarKinect::getThresholdControl() {
	return thresholdControl;
}