
#ifndef OFX_TOOGLE
#define OFX_TOOGLE

#include "ofxSimpleGuiMain.h"

//------------------------------------------------------------------------------ toggle
class ofxToggle : public ofMouseListener, public ofKeyListener {
	
public:
	
	float	posx, posy;
	
	bool*   value;
	string	name;
	
	bool	mouseon;
	bool	mouseover;
	bool	mousedown;
	bool	focused;
	
	int xmlID, guiID;
		
	//default copy overidring
	//ofImage(const ofImage& mom);						
	//ofxToggle& operator= (const ofxToggle& mom);
	
	//---------------------------------------------------------------------
	ofxToggle(string _name, bool *_value, int _xmlID=0, int _guiID=0) {
		
		xmlID = _xmlID;
		guiID = _guiID;
		
		focused   = false;
		mouseon   = false;
		mouseover = false;
		name      = _name;
		value     = _value;
		
		ofMouseEvents.addListener(this);
		ofKeyEvents.addListener(this);
	}
	
	//---------------------------------------------------------------------
	bool getValue() {
		return (*value);
	}
	void set(bool b) {
		(*value) = b;
	}
	void toggle() {
		(*value) = !(*value); 
	}
	//---------------------------------------------------------------------		
	void mouseMoved(int x, int y) {
		if(x > posx && x < posx+20 && y > posy && y < posy+20) {
			mouseover = true;
		}
		else {
			mouseover = false;
		}
	}
	
	void mousePressed(int x, int y, int button) {
		if(x > posx && x < posx+20 && y > posy && y < posy+20) {
			mouseon = true;
			(*value) = !(*value); 
		}
	}
	
	//void mouseReleased() {
	//		if(mouseon) {
	//			cout << name << ":" << value << endl;
	//			callback;
	//		}
	//		mouseon = false;
	//	}
	//---------------------------------------------------------------------
	void render(float _x, float _y) {
		
		posx = _x; posy = _y;
		
		glPushMatrix();
		glTranslatef(_x, _y, 0);
		
		ofEnableAlphaBlending();
		ofSetColor(SLIDER_R, SLIDER_G, SLIDER_B, 200);
		if(mouseover)				ofSetColor(255, 90, 25, 100);
		if(mouseon || focused)		ofSetColor(255, 90, 25, 200);
		
		ofRect(0, 0, 20, 20);
		
		if((*value)) {
			ofSetColor(255, 255, 255, 200);
			ofLine(0, 0, 20, 20);
			ofLine(20, 0, 0, 20);
		}
		
		ofSetColor(105, 105, 105, 200);
		if(mouseover) ofSetColor(105, 105, 105, 100);
		ofRect(25, 0, 200, 20);
		ofSetColor(0xffffff);
		ofDrawBitmapString(name, 35, 15);
		ofDisableAlphaBlending();
		
		glPopMatrix();
	}
	
};
#endif