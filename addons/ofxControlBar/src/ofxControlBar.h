/*
 *  ofxControlBar.h
 *  Control
 *
 *  Created by Josh Nimoy on 2/11/11.
 *
 */
#pragma once
#include "ofMain.h"
#include <deque>
#include <string>

using namespace std;

namespace ofxControlBar{

	class Event;
	
	/**
		superclass for real widgets
	 */
	class Control{
	public:
		Control();
		virtual ~Control();
		ofRectangle rect;
		deque<Control*> children;
		virtual void draw();
		virtual void update(int mouseX,int mouseY);
		bool mouseIsHovering;
		bool mouseIsDown;
		static deque<Event> eventQueue;
		virtual bool mouseDown();
		virtual void mouseUp();
		string className;
		void* userData;
		bool enabled;
		virtual void disable();
		virtual void enable();
	};

	
	/**
		event object
	 */
	class Event{
	public:
		Event(Control *it,string w);
		Control *obj;
		string what;
	};
	
	
	class Bar:public Control{
	public:
		Bar();
		~Bar();
		void draw();
		void update(int mouseX,int mouseY);
		int margin;
		void doLayout();
	};
	
	class Label:public Control{
	public:
		Label();
		Label(string txt);
		~Label();
		void draw();
		string text;
	};
	
	class Button:public Control{
	public:
		Button();
		Button(string txt);
		~Button();
		void draw();
		string text;
		int margin;
	};
	
	void drawRoundedRect(GLuint glMode,float w,float h,float r);
	
}
