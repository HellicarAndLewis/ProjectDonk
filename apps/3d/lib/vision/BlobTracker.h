#pragma once

#include "BlobListener.h"

class BlobTracker {
public:
	void addListener(BlobListener *listener) {
		listeners.push_back(listener);
	}
	
	void update(ofxCvContourFinder &contourFinder) {
		
		// we need width+height for normalizing
		dims = ofPoint(contourFinder.getWidth(), contourFinder.getHeight());
		
		for(int i = 0; i < contourFinder.nBlobs; i++) {
			notifyBlobMoved(0, contourFinder.blobs[i]);
		}
	}
private:
	void notifyBlobMoved(int blobId, ofxCvBlob &blob) {
		for(int i = 0; i < listeners.size(); i++) {
			listeners[i]->blobMoved(blobId, blob.centroid/dims);
		}
	}
	vector<BlobListener*> listeners;
	ofPoint dims;
};