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


static void drawSphere( const ofVec3f &center, float radius, int segments=10 ) {
	
	if( segments < 0 ) return;
	
	float *verts = new float[(segments+1)*2*3];
	float *normals = new float[(segments+1)*2*3];
	float *texCoords = new float[(segments+1)*2*2];
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, verts );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );
	glEnableClientState( GL_NORMAL_ARRAY );
	glNormalPointer( GL_FLOAT, 0, normals );
	
	for( int j = 0; j < segments / 2; j++ ) {
		float theta1 = j * 2 * 3.14159f / segments - ( 3.14159f / 2.0f );
		float theta2 = (j + 1) * 2 * 3.14159f / segments - ( 3.14159f / 2.0f );
		
		for( int i = 0; i <= segments; i++ ) {
			ofVec3f e, p;
			float theta3 = i * 2 * 3.14159f / segments;
			
			e.x = cos( theta1 ) * cos( theta3 );
			e.y = sin( theta1 );
			e.z = cos( theta1 ) * sin( theta3 );
			p = e * radius + center;
			normals[i*3*2+0] = e.x; normals[i*3*2+1] = e.y; normals[i*3*2+2] = e.z;
			texCoords[i*2*2+0] = 0.999f - i / (float)segments; texCoords[i*2*2+1] = 0.999f - 2 * j / (float)segments;
			verts[i*3*2+0] = p.x; verts[i*3*2+1] = p.y; verts[i*3*2+2] = p.z;
			
			e.x = cos( theta2 ) * cos( theta3 );
			e.y = sin( theta2 );
			e.z = cos( theta2 ) * sin( theta3 );
			p = e * radius + center;
			normals[i*3*2+3] = e.x; normals[i*3*2+4] = e.y; normals[i*3*2+5] = e.z;
			texCoords[i*2*2+2] = 0.999f - i / (float)segments; texCoords[i*2*2+3] = 0.999f - 2 * ( j + 1 ) / (float)segments;
			verts[i*3*2+3] = p.x; verts[i*3*2+4] = p.y; verts[i*3*2+5] = p.z;
		}
		glDrawArrays( GL_TRIANGLE_STRIP, 0, (segments + 1)*2 );
	}
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	
	delete [] verts;
	delete [] normals;
	delete [] texCoords;
}
