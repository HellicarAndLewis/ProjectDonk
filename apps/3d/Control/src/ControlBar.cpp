/*
 *  ControlBar.cpp
 *  Control
 *
 *  Created by Josh Nimoy on 2/11/11.
 
 quick and dirty UI with buttons, labels, and a single layout pane called a "bar"
 which sits at the top of the gl window. event passing only works for non-overlapping controls.
 
 *
 */

#include "ControlBar.h"

#define BUTTON_DEFAULT_MARGIN 10
#define BUTTON_RECT_RADIUS 6

namespace ControlBar{

	Event::Event(Control *it,string w){
		obj = it;
		what = w;
	}
	
	deque<Event> Control::eventQueue;
	
	
	Button::Button():Control(){
		className = "Button";
		margin = BUTTON_DEFAULT_MARGIN;
		text = "Click Here";
	}
	
	Button::Button(string txt):Control(){
		className = "Button";
		margin = BUTTON_DEFAULT_MARGIN;
		text = txt;
		rect.height = 20;
		rect.width = text.size() * 8 + margin;
	}
	
	Button::~Button(){
	}
	
	void Button::draw(){
		glPushMatrix();
		glTranslatef(rect.x,rect.y,0);
		
		glColor3f(0.25,0.25,0.25);
		if(enabled){
			if(mouseIsHovering)glColor3f(0.4,0.4,0.4);
			if(mouseIsDown)glColor3f(1,0.8,0);
			drawRoundedRect(GL_TRIANGLE_FAN,rect.width,rect.height,BUTTON_RECT_RADIUS);
		
			if(mouseIsHovering){
				if(mouseIsDown){
					ofSetColor(255,255,220);
				}else{
					ofSetColor(0,0,0);
				}
			}else{
				ofSetColor(150,150,150);
			}
		}
		
		drawRoundedRect(GL_LINE_LOOP,rect.width,rect.height,BUTTON_RECT_RADIUS);
		
		ofDrawBitmapString(text, ofPoint(margin/2,14));
		glPopMatrix();
	}
	

	
	//--------------------------------------------------------------------

	Label::Label():Control(){
		className = "Label";

	}
	
	Label::Label(string txt):Control(){
		className = "Label";
		text = txt;
		rect.height = 20;
		rect.width = text.size() * 8;
		
	}
	
	Label::~Label(){
	}
	
	void Label::draw(){
		glPushMatrix();
		glTranslatef(rect.x,rect.y,0);
		ofSetColor(150,150,150);
		ofDrawBitmapString(text, ofPoint(0,14));
		glPopMatrix();
	}
	
	//--------------------------------------------------------------------

	
	Control::Control(){
		rect.x = 0;
		rect.y = 0;
		rect.width = 50;
		rect.height = 20;
		mouseIsHovering = false;
		mouseIsDown = false;
		className = "Control";
		userData = NULL;
		enabled = true;
	}
	Control::~Control(){
		for(int i=0;i<children.size();i++){
			delete children[i];
		}
	}
	
	void Control::enable(){
		enabled = true;
	}
	
	void Control::disable(){
		enabled = false;
	}
	
	void Control::draw(){
		glColor3f(0.3,0.3,0.3);
		glRectf(rect.x,rect.y,rect.x+rect.width,rect.y+rect.height);
		
		glColor3f(1,1,1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(rect.x , rect.y);
		glVertex2f(rect.x+rect.width , rect.y);
		glVertex2f(rect.x+rect.width , rect.y+rect.height);
		glVertex2f(rect.x , rect.y+rect.height);
		glEnd();
		
		for(int i=0;i<children.size();i++){
			glPushMatrix();
			children[i]->draw();
			glPopMatrix();
		}
	}
	void Control::update(int mouseX,int mouseY){
		if(enabled){
			mouseIsHovering = (mouseX > rect.x && mouseX < rect.x+rect.width &&
			   mouseY > rect.y && mouseY < rect.y+rect.height);
			for(int i=0;i<children.size();i++)children[i]->update(mouseX,mouseY);
		}
	}
	
	bool Control::mouseDown(){
		if(enabled){
			mouseIsDown = false;
			
			for(int i=0;i<children.size();i++){
				if(children[i]->mouseDown())return true;
			}
			if(mouseIsHovering){
				mouseIsDown = true;
				return true;
			}
		}
		return false;
	}
	
	void Control::mouseUp(){
		if(enabled){
			for(int i=0;i<children.size();i++){
				children[i]->mouseUp();
			}
			if(mouseIsDown && mouseIsHovering){
				eventQueue.push_back(Event(this,"mouseClicked"));
			}
		}		
		mouseIsDown = false;
	}
	
	//--------------------------------------------------------------------
	
	Bar::Bar():Control(){
		className = "Bar";
		margin = 5;
		rect.height = 30;
		rect.width = ofGetWidth();
		doLayout();
	}	

	void Bar::doLayout(){
		int curX = margin;
		int curY = margin;
		for(int i=0;i<children.size();i++){
			children[i]->rect.x = curX;
			children[i]->rect.y = curY;
			curX += children[i]->rect.width + margin;
		}
	}
	
	Bar::~Bar(){
		
	}	

	void Bar::draw(){
		glColor3f(0.3,0.3,0.3);
		glRectf(0,0,ofGetWidth(),rect.height);
		
		for(int i=0;i<children.size();i++){
			glPushMatrix();
			children[i]->draw();
			glPopMatrix();
		}
	}

	void Bar::update(int mouseX,int mouseY){
		rect.width = ofGetWidth();
		Control::update(mouseX,mouseY);
	}
	
	
	
	
	void drawRoundedRect(GLuint glMode,float w,float h,float r){
		float hr = r/2;		
		glBegin(glMode);
		
		glVertex2f(0,hr-1);
		glVertex2f(hr-1,0);
		
		glVertex2f(w-hr,0);
		glVertex2f(w,hr);
		
		glVertex2f(w,h-hr);
		glVertex2f(w-hr,h);
		
		glVertex2f(hr-1,h);
		glVertex2f(0,h-hr+1);
		glEnd();
		
	}
}
