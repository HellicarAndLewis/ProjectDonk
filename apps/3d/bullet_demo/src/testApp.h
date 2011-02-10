#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#define BIT(x) (1<<(x))

struct callback : public btOverlapFilterCallback
{
	// return true when pairs need collision
	bool	needBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const;
};

class testApp : public ofBaseApp{

	
	
	
	public:
	
	
	int COL_NOTHING;
	int COL_SPHERE;
	
	int innerCollidesWith;
	int outerCollidesWith;
	
	
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	void initBullet();
	
	// just for testing
	btCompoundShape* containing;
	//btRigidBody* containingBody;
	
	vector<btRigidBody*> containingBubble;
	vector<btRigidBody*> containedBubble;
	
	
	///this is the most important class
	btDynamicsWorld*		m_dynamicsWorld;
	
	void updateBullet();
	void drawBullet();
	
	// this will create a containing bubble
	void createContainingBubble(ofVec3f origin, ofVec2f size);
	
	// this creates a child bubble
	btSphereShape* createContainedBubble(ofVec3f origin, float radius);
	
	ofVec4f btVecToOfVec4f( btVector4 bv );
	ofVec3f btVecToOfVec3f( btVector3 bv );
	
	btVector4 ofVec4fToBtVec( ofVec4f ov );	
	btVector3 ofVec3fToBtVec( ofVec3f ov );
		

};

#endif
