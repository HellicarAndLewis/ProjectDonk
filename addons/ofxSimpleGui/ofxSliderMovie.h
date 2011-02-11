
#ifndef OFX_SLIDER_MOVIE
#define OFX_SLIDER_MOVIE
#include "ofxSimpleGuiMain.h"
#include "ofxToggle.h"

//------------------------------------------------------------------------------ Movie slider
class ofxSliderMovie : public ofMouseListener, public ofKeyListener, public ofAppListener {
	
public:
	
	float				posx, posy;
	
	float				pct;
	float				sliderPos;
	
	float*				value;
	float				min, max;
	string				name;	
	bool				mouseOver, mouseDown;
	bool				lock;
	ofVideoPlayer*		input;
	
	float				videoLength;
	
	ofxToggle*			pauseBtn;
	bool				doPause;
	
	//---------------------------------------------------------------------
	ofxSliderMovie(string _name, ofVideoPlayer *_input, float *_value, float _min, float _max) {
		
		ofAppEvents.addListener(this);
		ofMouseEvents.addListener(this);
		ofKeyEvents.addListener(this);
		
		mouseOver = false;
		mouseDown = false;
		lock	  = false;
		
		name    = _name;
		value   = _value;
		min		= _min;
		max		= _max;
		input	= _input;
		
		//init
		if(_input) { 
			pct = ofMap((*value), min, max, 0.0, SLIDER_W);
			videoLength = input->getDuration();
		}
		
		//Buttons
//					toggles.push_back(new ofxToggle(_name, _value, _xmlID, guiCount));
		doPause = false;
		pauseBtn = new ofxToggle("Pause", &doPause, 0, 0);
	}
	
	//---------------------------------------------------------------------
	void update() {
		
		if(!input) return;
		
		if(pct > SLIDER_W) pct = SLIDER_W;
		else if( pct < 0)  pct = 0;
		
		float sliderPosPct = ofMap(input->getPosition(), 0.0, 1.0, 0.0, 100.0); 
		sliderPos = ofMap(sliderPosPct, 0.0, 100.0, 0.0, SLIDER_W);
		
	}
	
	//---------------------------------------------------------------------
	bool mouseTouch(int ix, int iy) {
		if(ix > posx && ix < posx+SLIDER_W && iy > posy && iy < posy+SLIDER_H) {
			return true;
		}
		else {
			return false;
		}
	}
	
	//---------------------------------------------------------------------
	void mouseMoved(int x, int y) {
		mouseOver = mouseTouch(x, y);
	}
	void mouseDragged(int x, int y, int buton) {
		if(lock) {
			
			sliderPos = x-posx;			
			input->setPosition(ofMap(x-posx, 0.0, SLIDER_W, 0.0, 100.0));
			input->setPaused(true);
		}
	}
	
	void mousePressed(int x, int y, int button) {
		
		input->setPaused(doPause);
		
		if(mouseTouch(x, y)) {
			
			mouseDown = true;
			input->setPaused(true);
			
			sliderPos = x-posx;			
			input->setPosition(ofMap(x-posx, 0.0, SLIDER_W, 0.0, 100.0));
			input->setPaused(true);
		}
	}
	void mouseReleased() {
		mouseDown = false;
		lock = false;
		input->play();
		input->setPaused(doPause);
	}
	//---------------------------------------------------------------------
	void render(float _x, float _y) {
		//update postion of gui object
		posx = _x;
		posy = _y;
		
		
		glPushMatrix();
		glTranslatef(posx, posy, 0);
		
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor(255, 255, 255, 200);
		ofRect(0, 0, SLIDER_W, SLIDER_H);
		
		
		ofSetColor(SLIDER_R, SLIDER_G, SLIDER_B, 200);
		if(mouseOver) ofSetColor(255, 90, 25, 200);
		ofDrawBitmapString(ofToString(sliderPos), sliderPos+20, SLIDER_H-15);
		ofRect(0, 0, sliderPos, SLIDER_H);
		
		ofSetColor(105, 105, 105, 200);
		ofRect(0, SLIDER_H, SLIDER_W, 25);
		ofSetColor(0xffffff);
		
		float pos = ofMap(input->getPosition(), 0.0, 1.0, 0.0, videoLength);
		ostringstream info;
		info << name << " " << ofToString(pos, 3.0) << "/" << ofToString(videoLength, 3.0) << endl;
		ofDrawBitmapString(info.str(), 3, SLIDER_H+15);
		
		//lines
		for(int i=0; i<videoLength; i+=3) {
			ofSetColor(200, 200, 200, 100);
			ofFill();
			float cx = ofMap(i, 0, videoLength, 0, SLIDER_W);
			ofLine(cx, 0, cx, SLIDER_H-1);
		}
		ofDisableAlphaBlending();
		glPopMatrix();
		
		//Buttons
		pauseBtn->render(posx, posy+50);
	}
	
};
#endif