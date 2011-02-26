/*
 *  ofxBulletHollowSphere.h
 *  Bullet
 *
 *
 */

#ifndef OFXBULLET_HOLLOW_SPHERE
#define OFXBULLET_HOLLOW_SPHERE

#include "ofxBullet.h"
#include "ofxBulletRigidBody.h"

#include "ofMain.h"

class ofxBulletHollowSphere
{
	
public:
	
	ofxBulletHollowSphere();
	
	void create(btDiscreteDynamicsWorld* world, float radius, ofVec3f position, int resolution);
	
	void translate(ofVec3f position);
	void rotate(ofVec3f position);
	void setCollisionFlags();
	void draw();
	
	vector<ofxBulletRigidBody*> getChildren() { return m_children; }
	
private:
	
	btDiscreteDynamicsWorld* m_world;
	vector<ofxBulletRigidBody*> m_children;

};

#endif