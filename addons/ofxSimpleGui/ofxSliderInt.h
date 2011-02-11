
#ifndef OFX_SLIDER_I
#define OFX_SLIDER_I

#include "ofxSimpleGuiMain.h"


class ofxSliderInt : public ofKeyListener, public ofMouseListener, public ofAppListener {
	
public:
	
	float		posx, posy;
	
	float		barwidth;
	float		pct;		
	int			*value;
	int			min, max;
	string		name;
	
	float		mx;
	float		my;
	bool		mouseon;
	bool		mouseover;
	bool		mousedown;
	bool		lock;
	bool		focused;
	
	int xmlID;
	int guiID;
	
	//--------------------------------------------------------------------- construct
	ofxSliderInt(string _name, int *_value, int _min, int _max, int _xmlID=0, int _guiID=0) {
		
		ofMouseEvents.addListener(this);
		ofKeyEvents.addListener(this);
		ofAppEvents.addListener(this);
		
		xmlID = _xmlID;
		guiID = _guiID;
		
		mouseon   = false;
		mouseover = false;
		mousedown = false;
		lock	  = false;
		focused   = false;
		
		name    = _name;
		value   = _value;
		min		= _min;
		max		= _max;
		
		//init
		pct = ofMap((*value), min, max, 0, SLIDER_W);
		barwidth = pct;
	}
	
	//--------------------------------------------------------------------- get value
	
	int getValue() {
		return (*value);
	}
	void set(int v) {
		(*value) = v;
	}
	void add() {
		pct ++;
		int temp = ofMap(pct, 0, SLIDER_W, min, max);
		
		//VALUE CLAMP
		if(temp >= max)			temp = max;
		else if(temp <= min)	temp = min;
		
		(*value) = temp;
		
	}
	void sub() {
		pct --;
		int temp = ofMap(pct, 0, SLIDER_W, min, max);
		
		//VALUE CLAMP
		if(temp >= max)			temp = max;
		else if(temp <= min)	temp = min;
		
		(*value) = temp;
		
	}
		
	//--------------------------------------------------------------------- update slider
	void updateSlider() {
		if(pct > SLIDER_W) {
			pct = SLIDER_W;
		}
		else {
			pct = mx-posx;
			int temp = ofMap(pct, 0, SLIDER_W, min, max);
			
			//VALUE CLAMP
			if(temp >= max)			temp = max;
			else if(temp <= min)	temp = min;
			
			(*value) = temp;
		}
	}
	
	//--------------------------------------------------------------------- on slider	
	bool onSlider() {
		if(mx > posx && mx < posx+SLIDER_W && my > posy && my < posy+SLIDER_H) {
			return true;
		}
		else return false;
	}
	
	//---------------------------------------------------------------------	mouse moved
	void mouseMoved(int x, int y) {
		mx = x; my = y;
		if(onSlider()) {
			mouseover = true;
		}
		else {
			mouseover = false;
		}
	}
	
	//--------------------------------------------------------------------- mouse dragged
	void mouseDragged(int x, int y, int button) {
		mx = x; my = y;
	}
	
	//--------------------------------------------------------------------- mouse pressed
	void mousePressed(int x, int y, int button) {
		
		mx = x; my = y;
		mousedown = true;
		
		if(onSlider()) {
			updateSlider();
			lock = true;
		}
		
	}
	
	//--------------------------------------------------------------------- mouse released
	void mouseReleased() {
		mousedown = false;
		lock = false;
	}
	
	//--------------------------------------------------------------------- update
	void update() {
		if(lock) {
			updateSlider();
		}
		
		
	}
	
	//--------------------------------------------------------------------- render
	void render(float _x, float _y) {
		
		//update postion of gui object
		posx = _x;
		posy = _y;
		
		//VALUE CLAMP
		barwidth = (*value);
		if(barwidth >= SLIDER_W)	barwidth = SLIDER_W;
		else if(barwidth <= 0)		barwidth = 0;
		
		ofEnableAlphaBlending();
		glPushMatrix();
		glTranslatef(posx, posy, 0);
		ofFill();
		
		ofSetColor(255, 255, 255, 200);
		if(mouseover) ofSetColor(255, 255, 255, 120);
		ofRect(0, 0, SLIDER_W, SLIDER_H);
		
		ofSetColor(SLIDER_R, SLIDER_G, SLIDER_B, 200);
		if(mouseon || focused) ofSetColor(255, 90, 25, 200);
		ofRect(0, 0, barwidth, SLIDER_H);
		
		ofSetColor(105, 105, 105, 200);
		ofRect(0, SLIDER_H, SLIDER_W, 25);
		ofSetColor(0xffffff);
		ofDrawBitmapString(name+" ["+ofToString((*value))+"]", 3, SLIDER_H+15);
		ofDisableAlphaBlending();
		glPopMatrix();
	}
	
};
#endif