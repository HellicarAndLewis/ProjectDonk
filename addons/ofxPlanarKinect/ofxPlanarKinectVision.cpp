/**
 * ofxPlanarKinectVision.cpp
 * KinectTracker
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */


#include "ofxPlanarKinect.h"

void ofxPlanarKinect::preprocessSlice() {
	
		
	//////////////////////////////////////////////////
	// Create a CV Image of the thresholded interactive area
	//////////////////////////////////////////////////
	
	
	// to create the cv image
	int numInteractionPixels = (int)kinectWidth*interactionDepth;
	unsigned char *intArea = new unsigned char[numInteractionPixels];
	
	int iai = 0; // quicker to increment an int than calculate an index
	
	// loop through each row of pixels
	for(int i = sliceY - interactionDepth; i < sliceY; i++) {
		for(int j = 0; j < kinectWidth; j++) {
			intArea[iai] = interactionArea[iai]>threshold[j]?(254.f*interactionArea[iai]+1):0;
			iai++;
		}
	}
	
	// resize if we need to
	if(cvImage.getHeight()!=interactionDepth) {
		cvImage.clear();
		cvImage.allocate(kinectWidth, interactionDepth);
	}
	
	cvImage.setFromPixels(intArea, kinectWidth, interactionDepth);
	
	// try to get rid of some noise with blur.
	cvImage.blur();
	cvImage.blur();
	
	cvImage.blur();
	
	
	delete [] intArea;
}

void ofxPlanarKinect::findBlobs() {
	
	
	contourFinder.findContours(cvImage, minHandWidth*interactionDepth, 3*maxHandWidth*interactionDepth, 10, false);
	
	
	
	
	rawBlobs.clear();
	for(int i = 0; i < contourFinder.nBlobs; i++) {
		
		
		// ignore any things that we suspect are too big to be hands
		if(contourFinder.blobs[i].boundingRect.width>maxHandWidth) continue;
		
		// make sure the hand is properly in the interaction area
		if(contourFinder.blobs[i].boundingRect.height<30) continue;
		
		// to calculate the x coord  we're going to find the point 
		// on the contour with the biggest y
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
		
		// this is now an approximation of the x coord (y coord is not correct yet)
		ofVec2f finalCoord = (leftmostMaxY + rightmostMaxY)/2.f;
		
		// to find the y, we take the max in the bottom quarter of the blob.
		
		// modify the boundingRect of the blob to be the bottom quarter of the
		// bounding rect of the 
		ofRectangle roi = contourFinder.blobs[i].boundingRect;
		roi.height*=0.25;
		roi.y += roi.height*3;
		
		
		cvImage.setROI(roi);
		double minVal;
		double maxVal;
		cvMinMaxLoc(cvImage.getCvImage(), &minVal, &maxVal);
		

		
		finalCoord.y = maxVal;
		rawBlobs.push_back(finalCoord);
	}
	cvImage.resetROI();
	
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
