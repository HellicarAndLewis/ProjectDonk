/**
 * Mask.h
 * emptyExample
 *
 * Created by Marek Bereza on 12/03/2011.
 *
 */
#include "ofMain.h"
#include "ofxXmlSettings.h"

class ofxPolygonMask {

public:
	float gamma;
	float blendPower;
	float luminance;
	float x;
	float y;
	ofxPolygonMask() ;
	
	void setup(string file = "") ;

	void save(string file = "") ;

	
	void setTexture(ofTexture *tex);
	
	void draw(ofEventArgs &args);
	
	void toggleEditing();
	void setEnableEditing(bool editingEnabled);
	bool isEditing();
	
	void setPosition(float x, float y);
	void toggleEnabled();
	void setEnabled(bool enabled);
	bool isEnabled();
	
	void mousePressed(ofMouseEventArgs &m);
	void mouseDragged(ofMouseEventArgs &m);
	void mouseReleased(ofMouseEventArgs &m) ;
	
	
	void keyPressed(ofKeyEventArgs &k);
	void keyReleased(ofKeyEventArgs &k);
	
	
	void clear();
	
	
private:
	
	void checkWinding();
	void flipDirection();
	void calcBlendShape();
	int sqrDist(int x1, int y1, int x2, int y2);
	int insertPoint(ofVec3f p);	
	float distToCentre(ofVec3f a, ofVec3f b, ofVec3f p);
	
	// the 3rd point is the mask thickness at a specific point
	vector<ofVec3f> points;
	
	vector<ofVec3f> blendShape;
	
	// the point that the mouse is
	// currently pressing down on.
	int selectedPoint;
	
	// the last point in this Plane
	// that the mouse pressed down on.
	int lastSelectedPoint;
	
	// if one of the points in this plane is 
	// focused for arrow key control, this
	// will be the index of the point. -1 otherwise.
	int focusedPoint;
	
	bool selected;
	
	bool doPoint;
	bool doDelete;
	string filePath;
	bool editing;
	bool enabled;
	ofTexture *tex;
	ofShader shader;
	bool usingShader;
};
