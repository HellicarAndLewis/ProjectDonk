/**
 * ofxPlanarKinectVision.cpp
 * KinectTracker
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */


#include "ofxPlanarKinect.h"

void ofxPlanarKinect::preprocessSlice() {
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// start with at least the first pixel being a lowpass filtered value of all the pixels that are not 0
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	if(slice[0]==0) {
		float val = 0;
		for(int i = 1; i < kinectWidth; i++) {
			if(slice[i]!=0) {
				if(val==0) {
					val = slice[i];
				} else {
					val = val*lpf + ((float)slice[i])*(1.f - lpf);
				}
			}
		}
		slice[0] = val;
		if(val>255) printf("First slice pixel too big! %f\n", val);
	}
	
	
	
	
	

	
	////////////////////////
	// Distance filter
	////////////////////////
	// make lp be even
	int lp = lpf*10;
	lp *= 2;
	if(lp>0) {
		unsigned char *temp = new unsigned char[(int)kinectWidth];
		memcpy(temp, slice, (int)kinectWidth);
		// now lpf the slice
		for(int i = 0; i < lp; i++) {
			
			for(int i = 1; i < kinectWidth-1; i++) {
				slice[i] = temp[i-1]*0.3333 + temp[i]*0.33333 + temp[i]*0.33333;
			}
			unsigned char *a = temp;
			slice = temp;
			temp = a;
		}
		delete temp;
	}
	
	
	//////////////////////////
	// Time filter
	//////////////////////////
	
	memcpy(lastSlice, slice, kinectWidth);
	////////////////////////////////////////////////
	// flood fill any black out with previous pixels
	////////////////////////////////////////////////
	if(fillHoles) {
		for(int i = 1; i < kinectWidth; i++) {
			if(slice[i]==0) slice[i] = slice[i-1];
		}
	}
	
	
}

void ofxPlanarKinect::findBlobs() {
	
	rawBlobs.clear();
	
	// look through slice and decide where the blobs are.
	
	for(int i = 0; i < kinectWidth; i++) {
		// loop until we find a blob
		while(i<kinectWidth-1 && slice[i]<threshold[i]) {
			i++;
		}
		
		// consume the blob
		if(slice[i]>=threshold[i]) {
			int blobStart = i;
			// find the end point (and also the depth)
			float maxDepth = slice[i];
			int blobCentre = 0;
			while(i<kinectWidth-1 && slice[i]>threshold[i]) {
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
	
	
	// we need to offset the output rect by the inset - this is
	// for if we can't calibrate on the corners and need to inset
	// the calibration rect.
	outputQuad[0] = ofVec2f(inset,inset);
	outputQuad[1] = ofVec2f(1-inset,inset);
	outputQuad[2] = ofVec2f(1-inset,1-inset);
	outputQuad[3] = ofVec2f(inset,1-inset);
	
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
