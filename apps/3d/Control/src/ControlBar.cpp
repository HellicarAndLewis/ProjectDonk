/*
 *  ControlBar.cpp
 *  Control
 *
 *  Created by Josh Nimoy on 2/11/11.
 *
 */

#include "ControlBar.h"

#define BUTTON_DEFAULT_MARGIN 10

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
		
		if(mouseIsHovering){
			glColor3f(0.8,0.8,0.8);
		}else{
			glColor3f(0.3,0.3,0.3);
		}
		glRectf(0,0,rect.width,rect.height);
		
		if(mouseIsHovering){
			glColor3f(0,0,0);
		}else{
			glColor3f(1,1,1);
		}
		glBegin(GL_LINE_LOOP);
		glVertex2f(0,0);
		glVertex2f(rect.width , 0);
		glVertex2f(rect.width , rect.height);
		glVertex2f(0 , rect.height);
		glEnd();
		
		
		if(mouseIsHovering){
			ofSetColor(0,0,0);
		}else{
			ofSetColor(255,255,255);
		}
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
		ofSetColor(255,255,255);
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
		className = "Control";
	}
	Control::~Control(){
		for(int i=0;i<children.size();i++){
			delete children[i];
		}
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
		mouseIsHovering = (mouseX > rect.x && mouseX < rect.x+rect.width &&
		   mouseY > rect.y && mouseY < rect.y+rect.height);
		for(int i=0;i<children.size();i++)children[i]->update(mouseX,mouseY);
	}
	
	bool Control::mouseDown(){
		for(int i=0;i<children.size();i++){
			if(children[i]->mouseDown())return true;
		}
		if(mouseIsHovering){
			eventQueue.push_back(Event(this,"mouseDown"));
			return true;
		}
		return false;
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
}
