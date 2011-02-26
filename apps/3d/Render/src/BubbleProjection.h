/**
 * BubbleProjection.h
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */
#pragma once

#include "ProjectedRect.h"
#include "BubbleData.h"
#include "ofxBullet.h"
#include "ContentBubble.h"

class BubbleProjection: public ProjectedRect {
public:
	BubbleProjection();
	/** This gets called straight after allocate() */
	void setup();
	
	// TODO: exit()...
	// void exit(); <--- I think i might need an exit function....
	
	/** Called every frame */
	void update();
	void draw();
	
	/** 
	 * Touches come in normalized to the interactive area.
	 * i.e. (0,0) is the top left, (1,1) is bottom right.
	 */
	void touchDown(float x, float y, int touchId);
	void touchMoved(float x, float y, int touchId);
	void touchUp(float x, float y, int touchId);

	/** A bubble was just received */
	void bubbleReceived(Donk::BubbleData *bubbleData);
	
	// Other useful methods of the inherited class:
	// float getWidth();
	// float getHeight();
	
	
	
	
	/** Maps a normalized point to coordinates in the interactive area */
	ofVec2f mapToInteractiveArea(ofVec2f inPoint);
	
	/** This is the rectangle where the interaction is going to happen */
	ofRectangle &getInteractiveArea();
	
private:
	
	// Bullet
	ofxBullet				bullet;
	ofLight					light;
	ofCamera				camera;
	vector <ContentBubble*> bubbles;
	
	
	
	
	ofRectangle interactiveArea;

	/** this gets called (from touchDown) when there is a detected double touch */
	void doubleTouchGesture(int touch1Id, int touch2Id);
	
	/** This is a local copy of the current touches */
	map<int,ofVec2f> touches;
	
	/** Convenience so you don't have to keep typing it */
	map<int,ofVec2f>::iterator tIt;
};