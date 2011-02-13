/*
 *  ofxBulletUtils.h
 *  Bullet
 *
 *  Created by Todd Vanderlin on 2/5/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "btMotionState.h"

#define SCALE 1.0


static btVector3 btVec3Scaled(ofVec3f &v) {
	v /= SCALE;
	return btVector3(v.x, v.y, v.z);
}
static btVector3 tobtVec3(ofVec3f &v) {
	return btVector3(v.x, v.y, v.z);
}


static btVector4 ofVec4fToBtVec( ofVec4f ov ) {
	btVector4 bv(ov.x, ov.y, ov.z, ov.w);
	return bv;
}

static btVector3 ofVec3fToBtVec( ofVec3f ov ) {
	btVector3 bv(ov.x, ov.y, ov.z);
	return bv;
}

static ofVec4f btVecToOfVec4f( btVector4 bv ) {
	ofVec4f ov(bv.getX(), bv.getY(), bv.getZ(), bv.getW());
	return ov;
}

static ofVec3f btVecToOfVec3f( btVector3 bv ) {
	ofVec3f ov(bv.getX(), bv.getY(), bv.getZ());
	return ov;
}


class MyKinematicMotionState : public btMotionState {

public:
   
	MyKinematicMotionState(const btTransform &initialpos) {
        mPos1 = initialpos;	
    }
	
    virtual ~ MyKinematicMotionState() {
    }
	
	//    void setNode(Ogre::SceneNode *node) {
	//        mVisibleobj = node;
	//    }
	
    virtual void getWorldTransform(btTransform &worldTrans) const {
        worldTrans = mPos1;
    }
	
    void setKinematicPos(btTransform &currentPos) {
        mPos1 = currentPos;
    }
	
    virtual void setWorldTransform(const btTransform &worldTrans) {
    }
	
protected:
    btTransform mPos1;
};


static btVector3 * ofVec3TobtVec3(const ofVec3f * points, int nPts) {

	btVector3 btPts[nPts];
	
	if(points) {
		
		for (int i=0; i<nPts; i++) {
			btPts[i].setX(points[i].x);
			btPts[i].setY(points[i].y);
			btPts[i].setZ(points[i].z);
		}
		
	}
	
	return btPts;
}