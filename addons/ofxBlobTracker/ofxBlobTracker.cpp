/*
 *  ofxBlobTracker.cpp
 *  ofxBlobTrackerExample
 *
 *  Created by Marek Bereza on 31/01/2011.
 *
 */

#include "ofxBlobTracker.h"

ofxBlobTracker::ofxBlobTracker() {
	for (int i=0;i<NUM_KALMAN_POINTS;i++) {
		tuioPointSmoothed[i] = NULL;
	}
}

void ofxBlobTracker::addListener(ofxBlobListener *listener) {
	listeners.push_back(listener);
}


ofxBlob *ofxBlobTracker::updateKalman(int id, ofxBlob *blob) {
	if (id>=NUM_KALMAN_POINTS/2) return NULL;
	if(tuioPointSmoothed[id*2] == NULL) {
		tuioPointSmoothed[id*2] = new ofxCvKalman(blob->x);
		tuioPointSmoothed[id*2+1] = new ofxCvKalman(blob->y);
	} else {
		blob->x = tuioPointSmoothed[id*2]->correct(blob->x);
		blob->y = tuioPointSmoothed[id*2+1]->correct(blob->y);
	}
	
	return blob;
}

void ofxBlobTracker::clearKalman(int id) {
	if (id>=NUM_KALMAN_POINTS/2) return;
	if(tuioPointSmoothed[id*2]) {
		delete tuioPointSmoothed[id*2];
		tuioPointSmoothed[id*2] = NULL;
		delete tuioPointSmoothed[id*2+1];
		tuioPointSmoothed[id*2+1] = NULL;
	}
}


int ofxBlobTracker::getNextAvailableBlobId() {
	for(int id = 0; id < NUM_KALMAN_POINTS/2; id++) {
		bool foundId = false;
		for(int i = 0; i < lastBlobs.size(); i++) {
			if(lastBlobs[i]->id==id) {
				foundId = true;
				continue;
			}
			if(!foundId) {
				return id;
			}
		}
	}
	return 0;
}
ofxBlob *ofxBlobTracker::newBlob(ofVec3f coords) {
	ofxBlob *blob = new ofxBlob();
	blob->x = coords.x;
	blob->y = coords.y;
	blob->z = coords.z;
	blob->id = getNextAvailableBlobId();
	blob->touched = true;
	lastBlobs.push_back(blob);
	return blob;
}
void ofxBlobTracker::updateBlob(int id, ofVec3f *blob) {
	for(int i = 0; i < lastBlobs.size(); i++) {
		if(lastBlobs[i]->id==id) {
			lastBlobs[i]->x = blob->x;
			lastBlobs[i]->y = blob->y;
			lastBlobs[i]->z = blob->z;
			lastBlobs[i]->touched = true;
			return;
		}
	}
}
void ofxBlobTracker::untouchLastBlobs() {
	for(int i = 0; i < lastBlobs.size(); i++) {
		lastBlobs[i]->touched = false;
	}
}

void ofxBlobTracker::track(vector<ofVec3f> &blobs) {
	untouchLastBlobs();
	for(int i = 0; i < blobs.size(); i++) {
		ofxBlob *blob = getClosestBlob(blobs[i]);
		
		// new blob!
		if(blob==NULL || blob->distance(blobs[i])>0.2) {

			ofxBlob *b = newBlob(blobs[i]);
			updateKalman(b->id, b);
			notifyAllListeners(b->x, b->y, b->id, ofxBlobTracker_entered);
			
		// updated blob
		} else {
			blob = updateKalman(blob->id, blob);
			updateBlob(blob->id, &blobs[i]);
			notifyAllListeners(blob->x, blob->y, blob->id, ofxBlobTracker_moved);
		}
	}
	// dead blob
	// delete any untouched blobs
	for(int i = 0; i < lastBlobs.size(); i++) {
		if(lastBlobs[i]->touched==false) {
			notifyAllListeners(lastBlobs[i]->x, lastBlobs[i]->y, lastBlobs[i]->id, ofxBlobTracker_exited);
			delete lastBlobs[i];
			lastBlobs.erase(lastBlobs.begin()+i);
			i--;
		}
	}
}

ofxBlob *ofxBlobTracker::getClosestBlob(ofVec3f &blob) {
	if(lastBlobs.size()==0) return NULL;
	float minSqrDist = FLT_MAX;
	int minI = -1;
	for(int i = 0; i < lastBlobs.size(); i++) {
		float diffX = blob.x - lastBlobs[i]->x;
		float diffY = blob.y - lastBlobs[i]->y;
		float sqrDist = diffX*diffX + diffY*diffY;
		if(sqrDist<minSqrDist) {
			minI = i;
			minSqrDist = sqrDist;
		}
	}
	if(minI==-1) return NULL; // should never happen.
	return lastBlobs[minI];
}

void ofxBlobTracker::notifyAllListeners(float x, float y, int id, ofxBlobEventType type) {
	for(int i = 0; i < listeners.size(); i++) {
		switch(type) {
			case ofxBlobTracker_entered:
				listeners[i]->blobEntered(x, y, id);
				break;
			case ofxBlobTracker_moved:
				listeners[i]->blobMoved(x, y, id);
				break;
			case ofxBlobTracker_exited:
				listeners[i]->blobExited(x, y, id);
				break;
		}
	}
}