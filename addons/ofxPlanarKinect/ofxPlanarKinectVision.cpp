/**
 * ofxPlanarKinectVision.cpp
 * KinectTracker
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */


#include "ofxPlanarKinect.h"

void ofxPlanarKinect::preprocessSlice() {
	
	
	////////////////////////
	// Distance filter
	////////////////////////
	// make lp be even
	int lp = lpf*10;
	lp *= 2;


	if(lp>0) {
		
		float *t = new float[(int)kinectWidth];
		float *temp = t;
	
		memcpy(t, slice, (int)kinectWidth*sizeof(float));
		
		
		// now lpf the slice
		for(int i = 0; i < lp; i++) {
			
			for(int i = 1; i < kinectWidth-1; i++) {
				slice[i] = t[i-1]/3.f + t[i]/3.f + t[i+1]/3.f;
			}
			float *a = t;
			t = slice;
			slice = a;
		}
		
		delete [] temp;
		
		
	}
	
	//////////////////////////
	// Time filter
	//////////////////////////
	if(timeFilter>0) {
		for(int i = 0; i < kinectWidth; i++) {
			slice[i] = slice[i]*(1.f - timeFilter) + lastSlice[i]*timeFilter;
		}
		memcpy(lastSlice, slice, kinectWidth*sizeof(float));
	}
	
	
	//////////////////////////////////////////////////
	// Create a CV Image of the thresholded interactive area
	//////////////////////////////////////////////////
	
	int numInteractionPixels = (int)kinectWidth*interactionDepth;
	unsigned char *intArea = new unsigned char[numInteractionPixels];
	
	int iai = 0; // quicker to increment an int than calculate an index
	
	// loop through each row of pixels
	for(int i = sliceY - interactionDepth; i < sliceY; i++) {
		for(int j = 0; j < kinectWidth; j++) {
			intArea[iai] = interactionArea[iai]>threshold[j]?255:0;
			iai++;
		}
	}
	
	// resize if we need to
	if(cvImage.getHeight()!=interactionDepth) {
		cvImage.clear();
		cvImage.allocate(kinectWidth, interactionDepth);
	}
	
	cvImage.setFromPixels(intArea, kinectWidth, interactionDepth);
	
	
	
}

void ofxPlanarKinect::findBlobs() {
	
	contourFinder.findContours(cvImage, minHandWidth*interactionDepth, 3*maxHandWidth*interactionDepth, 10, false);
	
	rawBlobs.clear();
	for(int i = 0; i < contourFinder.nBlobs; i++) {
		if(contourFinder.blobs[i].boundingRect.width>maxHandWidth) continue;
		
		
		// to calculate the more accurate version of the x coord 
		// we're going to find the point on the contour with the 
		// biggest y
		float maxY = 0;
		
		ofVec2f leftmostMaxY(0, 0);
		ofVec2f rightmostMaxY(0, 640);
		

		for(int j = 0; j < contourFinder.blobs[i].nPts; j++) {
			ofPoint pt = contourFinder.blobs[i].pts[j];
			if(pt.y>maxY) {
				maxY = pt.y;
				leftmostMaxY = ofVec2f(pt.x, pt.y);
				rightmostMaxY = leftmostMaxY;
			} else if(pt.y==maxY) {
				if(pt.x<leftmostMaxY.x) leftmostMaxY.x = pt.x;
				else if(pt.x>rightmostMaxY.x) rightmostMaxY.x = pt.x;
			}
		}
		
		
		
		ofVec2f finalCoord = (leftmostMaxY + rightmostMaxY)/2.f;
		

		// sample the pixels in an inverted cross
		//     X
		//     X
		//     X
		//    XXX
		//    XXX
		//     X
		int index = finalCoord.x + ((sliceY - interactionDepth)+(int)finalCoord.y)*kinectWidth;
		float valuesToMax[10];
		valuesToMax[0] = currFrame[index];
		valuesToMax[1] = currFrame[index-1];
		valuesToMax[2] = currFrame[index+1];
		valuesToMax[3] = currFrame[index-(int)kinectWidth];
		valuesToMax[4] = currFrame[index+(int)kinectWidth];
		valuesToMax[5] = currFrame[index-(int)kinectWidth*2];
		valuesToMax[6] = currFrame[index-(int)kinectWidth*3];
		valuesToMax[7] = currFrame[index-(int)kinectWidth*4];
		valuesToMax[8] = currFrame[index-1 - (int)kinectWidth];
		valuesToMax[9] = currFrame[index+1 - (int)kinectWidth];
		maxY = valuesToMax[0];
		
		float thresholdComparer = threshold[(int)finalCoord.x];
		
		for(int i = 1; i < 10; i++) {
			if(valuesToMax[i]>maxY) maxY = valuesToMax[i];
		}
		finalCoord.y = maxY;
		
		rawBlobs.push_back(finalCoord);
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
