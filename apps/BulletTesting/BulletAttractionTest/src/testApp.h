
#pragma once
#include "ofMain.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "ofxBullet.h"





class ofxBulletDebugDrawer : public btIDebugDraw {
	
private:
	
	int m_debugMode;
public:
	
	
	ofxBulletDebugDrawer() {
	}
	
	
	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor) {
		printf("sphere draw\n");
	}
	
	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color) {
		printf("sphere draw\n");
	}
	
	virtual void	drawSphere (const btVector3& p, btScalar radius, const btVector3& color) {
		printf("sphere draw\n");
	}
	
	virtual void	drawBox (const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha) {
		printf("sphere draw\n");
	}
	
	virtual void	drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha) {
		printf("sphere draw\n");
	}
	
	virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) {
		printf("sphere draw\n");
	}
	
	virtual void	reportErrorWarning(const char* warningString) {
		printf("sphere draw\n");
	}
	
	virtual void	draw3dText(const btVector3& location,const char* textString) {
		printf("sphere draw\n");
	}
	
	void	setDebugMode(int debugMode) {
		printf("sphere draw\n");
	}
	
	int		getDebugMode() const { return m_debugMode;}
	
	
};




class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	bool bApplyForce;
	ofLight light;
	
	ofxBullet bullet;
	ofxBulletRigidBody * mom;
	ofxBulletRigidBody * mom2;
};
