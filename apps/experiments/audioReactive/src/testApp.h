/*
 *  MyApp.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */
#pragma once
#include "App.h"
#include "Bubbles.h"

/**
 * Just like the regular testApp, but it implements App instead of ofBaseApp.
 * Instead of implementing draw(), you implement drawView(), which gets called
 * for as many projectors as you have configured. You also have render(), which
 * you don't use to draw to the screen, but to any FBO's which might be needed
 * by drawView() - so you don't have to draw them for each camera.
 */
class testApp: public App {
public:
	
	
	// custom stuff
	void drawFace(vector<ofPoint> &points);
	
	// inherited methods
	void setup();
	void update();

	void render();
	void drawView();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void audioReceived(float *input, int bufferSize, int numChannels);

protected:
	float volume;
	Bubbles bubbles;
	
};