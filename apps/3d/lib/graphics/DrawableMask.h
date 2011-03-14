/**
 * Mask that takes mouse and key events to allow you to draw it on the fly
 */
#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"


class DrawableMask {
public:

	vector<ofPoint> points;
	
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
	
	// the name of the Plane in the xml
	string name;
	
		
	bool selected;
	
	
	bool doPoint;
	string filePath;
	
	DrawableMask()
	{
		maskMode = false;
		filePath = "";
		doPoint = false;
		doDelete = false;
		name = "";
		selectedPoint = lastSelectedPoint = focusedPoint = -1;

		selected = false;
	}
	
	DrawableMask(string _name) {
		doPoint = false;
		doDelete = false;
		maskMode = false;
		filePath = "";
		name = _name;
		selectedPoint = lastSelectedPoint = focusedPoint = -1;

		selected = false;
	}
	
	void load(string file = "") {
		if(file=="" && filePath=="") {
			printf("DrawableMask: Couldn't write file because no path was specified\n");
			return;
		}
		if(file!="") filePath = file;
		ofxXmlSettings xml;
		xml.loadFile(file);
		xml.pushTag("points");
		points.clear();
		int numTags = xml.getNumTags("point");
		for(int i = 0; i < numTags; i++) {

			points.push_back(
							 ofPoint(
									 xml.getAttribute("point", "x", 0.f, i),
									 xml.getAttribute("point", "y", 0.f, i)
							)
			);
		}
	}
	
	void save(string file = "") {
		if(file=="" && filePath=="") {
			printf("DrawableMask: Couldn't write file because no path was specified\n");
			return;
		}
		if(file!="") filePath = file;
		ofxXmlSettings xml;
		xml.addTag("points");
		xml.pushTag("points");
		for(int i = 0; i < points.size(); i++) {
			xml.addTag("point");
			xml.addAttribute("point", "x", points[i].x, i);
			xml.addAttribute("point", "y", points[i].y, i);
		}
		xml.saveFile(filePath);

	}
	
	
	
	ofPoint *focusLastPoint() {
		focusedPoint = lastSelectedPoint;
		return &points[focusedPoint];
	
	}
	
	void unfocusLastPoint() {
		focusedPoint = -1;
	}
	
	
	
	void draw() {

		
		if(maskMode) {
			
			ofNoFill();
			for(int i = 0; i < points.size(); i++) {
				if(selectedPoint==i || focusedPoint==i) ofSetHexColor(selected?0x00FF00:0x009900);
				else ofSetHexColor(0xFF0000);
				ofCircle(points[i].x, points[i].y, 5);
			}
			
			ofSetHexColor(0xFF0000);
			ofBeginShape();
			for(int i = 0; i < points.size(); i++) {
				ofVertex(points[i].x, points[i].y);
			}
			ofEndShape(true);
			
			
			ofFill();
			ofEnableAlphaBlending();
			ofSetColor(255, 0, 0, 120);
			ofBeginShape();
			for(int i = 0; i < points.size(); i++) {
				ofVertex(points[i].x, points[i].y);
			}
			ofEndShape(true);
			
		} else {
			ofFill();
			ofSetColor(0, 0, 0);
			ofBeginShape();
			for(int i = 0; i < points.size(); i++) {
				ofVertex(points[i].x, points[i].y);
			}
			ofEndShape(true);
		}
	}
	
	void setMaskMode(bool _maskMode) {
		maskMode = _maskMode;
	}
	
	int sqrDist(int x1, int y1, int x2, int y2) {
		int distX = x1 - x2;
		int distY = y1 - y2;
		return distX*distX + distY*distY;
	}
	
	bool mouseDown(int x, int y) {
		if(doPoint) {
			points.push_back(ofPoint(x, y));
		} else if(doDelete) {
			for(int i = 0; i < points.size(); i++) {
				if(sqrDist(x, y, points[i].x, points[i].y)<25) { // 5 pixels distance
					points.erase(points.begin()+i);
					return true;
				}
			}
			
		}
		for(int i = 0; i < points.size(); i++) {
			if(sqrDist(x, y, points[i].x, points[i].y)<25) { // 5 pixels distance
				selectedPoint = i;
				points[selectedPoint].x = x;
				points[selectedPoint].y = y;
				return true;
			}
		}
		return false;
	}

	bool mouseDragged(int x, int y) {
		if(selectedPoint!=-1) {
			points[selectedPoint].x = x;
			points[selectedPoint].y = y;

			return true;
		} else {
			return false;
		}
	}
	
	bool mouseUp(int x, int y) {
		if(selectedPoint!=-1) {
			lastSelectedPoint = selectedPoint;
			selectedPoint = -1;
			save();
			return true;
		} else {
			save();
			return false;
		}
	}
	

	void keyPressed(int key) {
		if(key=='\'') {
			doPoint = true;
		} else if(key=='\\') {
			doDelete = true;
		}
	}
	
	void keyReleased(int key) {
		if(key=='\'') {
			doPoint = false;
		} else if(key=='\\') {
			doDelete = false;
		}
	}
	
		
	
	bool pointInsidePlane(ofPoint &p){
		int counter = 0;
		int i;
		double xinters;
		ofPoint p1,p2;
		
		int N = points.size();
		
		p1 = points[0];
		for (i=1;i<=N;i++) {
			p2 = points[i % N];
			if (p.y > MIN(p1.y,p2.y)) {
				if (p.y <= MAX(p1.y,p2.y)) {
					if (p.x <= MAX(p1.x,p2.x)) {
						if (p1.y != p2.y) {
							xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
							if (p1.x == p2.x || p.x <= xinters)
								counter++;
						}
					}
				}
			}
			p1 = p2;
		}
		
		if (counter % 2 == 0) return false;
		else return true;
	}
	
	/**
	 * Changes the direction of all the vertices. 
	 * i.e. clockwise to anti-clockwise
	 */
	void flipDirection() {
		
		int a=0;
		int b = points.size();
		ofPoint swap;
		
		for(int a = 0; a<--b; a++) //increment a and decrement b until they meet eachother
		{
			swap = points[a];       //put what's in a into swap space
			points[a] = points[b];    //put what's in b into a
			points[b] = swap;       //put what's in the swap (a) into b
		}
		
		
	}
	
	
	/**
	 * Shifts the points along one
	 */
	void shiftPointsLeft() {
		ofPoint first = points[0];
		for(int i = 1; i < points.size(); i++) {
			points[i-1] = points[i];
		}
		points[points.size()-1] = first;
	}
	
	void shiftPointsRight() {
		ofPoint first = points[points.size()-1];
		for(int i = points.size()-1; i > 1; i--) {
			points[i-1] = points[i];
		}
		points[0] = first;
	}
	bool maskMode;
	bool doDelete;
};
