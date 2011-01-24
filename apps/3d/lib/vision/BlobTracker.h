#pragma once

#include "BlobListener.h"

class BlobTracker {
public:
	void addListener(BlobListener *listener) {
		listeners.push_back(listener);
	}
	
	void update(vector<ofVec3f> &blobs) {
		
		
		for(int i = 0; i < blobs.size(); i++) {
			notifyBlobMoved(0, blobs[i]);
		}
	}
private:
	void notifyBlobMoved(int blobId, ofVec3f &blob) {
		for(int i = 0; i < listeners.size(); i++) {
			listeners[i]->blobMoved(blobId, blob);
		}
	}
	vector<BlobListener*> listeners;
};