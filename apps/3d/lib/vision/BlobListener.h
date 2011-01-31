/*
 *  KinectListener.h
 *  KinectExample
 *
 *  Created by Marek Bereza on 24/01/2011.
 *
 */
#pragma once
#include "ofMain.h"
class BlobListener {
public:
	virtual void blobMoved(int blobId, ofPoint centroid) {}
};