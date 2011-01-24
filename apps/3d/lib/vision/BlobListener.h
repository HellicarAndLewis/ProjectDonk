/*
 *  KinectListener.h
 *  KinectExample
 *
 *  Created by Marek Bereza on 24/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"
class BlobListener {
public:
	virtual void blobMoved(int blobId, ofPoint centroid) {}
};