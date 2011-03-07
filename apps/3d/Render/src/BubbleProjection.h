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
#include "TouchedConstraint.h"
#include "BubbleShader.h"

#include "InteractionBuzz.h"
#include "InteractionInspiration.h"
#include "InteractionInterview.h"
#include "InteractionChoice.h"
#include "InteractionPerformance.h"

class BubbleProjection: public ProjectedRect {

private:
	BubbleProjection();
	
public:

	static BubbleProjection *getInstance() {
		static BubbleProjection *instance = NULL;
		if(instance==NULL) instance = new BubbleProjection();
		return instance;
	}
	
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

	//--------------------------------------------------------
	void bubbleReceived(Donk::BubbleData *bubbleData);
	void interactionModeChange(string modeName); 

	// Other useful methods of the inherited class:
	// float getWidth();
	// float getHeight();	
	
	/** Maps a normalized point to coordinates in the interactive area */
	ofVec2f mapToInteractiveArea(ofVec2f inPoint);
	
	/** This is the rectangle where the interaction is going to happen */
	ofRectangle &getInteractiveArea();
	
private:
	
	//--------------------------------------------------------
	// Bullet
	ofxBullet					bullet;
	ofCamera					camera; // not using anymore...
	vector <TouchedConstraint*> touchConstraints;
	
	//--------------------------------------------------------
	// Interactions
	vector <BaseInteraction*>	interactions;
	BaseInteraction *			activeInteraction;
	BaseInteraction *			previousInteraction;
	
	// Sphere rendering
	//--------------------------------------------------------
	BubbleShader				bubbleShader;
	
	//--------------------------------------------------------
	void addTouchConstraints(ContentBubble * bubble);
	void removeTouchConstraint(ContentBubble * bubble);
	
	//--------------------------------------------------------	
	ofRectangle interactiveArea;

	/** this gets called (from touchDown) when there is a detected double touch */
	void doubleTouchGesture(int touch1Id, int touch2Id);
	
	/** This is a local copy of the current touches */
	map<int,ofVec2f> touches;
	
	/** Convenience so you don't have to keep typing it */
	map<int,ofVec2f>::iterator tIt;
};