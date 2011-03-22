/**
 * ofxPolygonMask.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 12/03/2011.
 *
 */
#include "ofxPolygonMask.h"
#define SQR_DIST_POINT_GRAB 36 // 6 pixels	
#define DIST_POINT_GRAB 6
#include <vector>
ofxPolygonMask::ofxPolygonMask() {
	filePath = "";
	doPoint = false;
	doDelete = false;
	
	selectedPoint = lastSelectedPoint = focusedPoint = -1;
	gamma = 1.0;	
	blendPower = 1.0;
	luminance = 1.0;
	selected = false;
	enabled = false;
	editing = false;
	tex = NULL;
	usingShader = false;
	x = y = 0;
}

void ofxPolygonMask::setTexture(ofTexture *tex) {
	if(!usingShader) {
		usingShader = true;
		shader.setup("shader/polygonblend.vert", "shader/polygonblend.frag", "");
		printf("Loaded shader\n");
	}
	this->tex = tex;
}

void ofxPolygonMask::setup(string file) {
	
	if(file=="" && filePath=="") {
		printf("ofxPolygonMask: Couldn't write file because no path was specified\n");
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
						 ofVec3f(
								 xml.getAttribute("point", "x", 0.f, i),
								 xml.getAttribute("point", "y", 0.f, i),
								 xml.getAttribute("point", "thickness", 0.f, i)
								 )
						 );
	}
	setEnabled(enabled);
	setEnableEditing(editing);
}

void ofxPolygonMask::save(string file) {
	if(file=="" && filePath=="") {
		printf("ofxPolygonMask: Couldn't write file because no path was specified\n");
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
		xml.addAttribute("point", "thickness", points[i].z, i);
	}
	xml.saveFile(filePath);
	
}


void ofxPolygonMask::checkWinding() {
	if(points.size()<3) return;
	ofVec2f a = points[1] - points[0];
	ofVec2f b = points[2] - points[0];
	float z = (a.x*b.y) - (a.y*b.x);
	
	// if the z of the cross product is negative, we need to reverse the winding
	if(z<0) {
		flipDirection();
	}
	
}

void ofxPolygonMask::setPosition(float x, float y) {
	this->x = x;
	this->y = y;
}

void ofxPolygonMask::draw(ofEventArgs &args) {
	
	
	if(points.size()==0) return;

	if(points.size()<4) checkWinding();

	glPushMatrix();
	glTranslatef(x, y, 0);
	if(usingShader) {		
		

		shader.begin();
		shader.setUniformTexture("tex", *tex, 0);
		shader.setUniform1f("gamma", gamma);
		shader.setUniform1f("blendPower", blendPower);
		shader.setUniform1f("luminance", luminance);
		shader.setUniform1f("height", tex->getHeight());
		shader.setUniform1i("flip", 1);
		shader.setUniform2f("offset", x, y);
	}
	
	ofFill();
	ofSetColor(0, 0, 0);
	
	// do a normal fill of the inside
	ofBeginShape();
	
	for(int i = 0; i < points.size(); i++) {
		ofVertex(points[i].x, points[i].y);
	}
	
	ofEndShape(true);
	
	calcBlendShape();
	ofEnableAlphaBlending();
	if(points.size()>0) {
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < points.size(); i++) {
			glColor4f(0, 0, 0, 1);
			glVertex2f(points[i].x, points[i].y);
			glColor4f(0, 0, 0, 0);
			glVertex2f(blendShape[i].x, blendShape[i].y);
		}
		glColor4f(0, 0, 0, 1);
		glVertex2f(points[0].x, points[0].y);
		glColor4f(0, 0, 0, 0);
		glVertex2f(blendShape[0].x, blendShape[0].y);
		
		glEnd();
	}
	
	if(usingShader) {
		shader.end();
	}
	
	if(editing) {
		ofNoFill();
		
		ofSetHexColor(0xFF0000);
		// do a normal fill of the inside
		ofBeginShape();
		for(int i = 0; i < points.size(); i++) ofVertex(points[i].x, points[i].y);
		ofEndShape(true);
		for(int i = 0; i < points.size(); i++) {
			if(i==focusedPoint) ofSetHexColor(0x00FF00);
			else ofSetHexColor(0xFF0000);
			ofCircle(points[i].x, points[i].y, DIST_POINT_GRAB);
		}
		
		ofSetHexColor(0xCC0000);
		ofBeginShape();
		for(int i = 0; i < blendShape.size(); i++) ofVertex(blendShape[i].x, blendShape[i].y);
		ofEndShape(true);
		ofFill();
	}
	glPopMatrix();
	
}

void ofxPolygonMask::calcBlendShape() {

	blendShape.clear();
	
	// do the outer blend
	for(int i = 0; i < points.size(); i++) {

		// a is the point before, b is the current point, c is the one after
		ofVec2f a = points[i==0?points.size()-1:i-1];
		ofVec2f b = points[i];
		ofVec2f c = points[i<points.size()-1?i+1:0];
		
		c = c - b;
		a = b - a;
		
		a.normalize();
		c.normalize();
		
		
		ofVec2f m = (a + c)*0.5;
		
		float tmp = m.y;
		m.y = -m.x;
		m.x = tmp;
		m.normalize();
		
		m *= points[i].z;
		
		blendShape.push_back(b+m);
	}
}


int ofxPolygonMask::sqrDist(int x1, int y1, int x2, int y2) {
	int distX = x1 - x2;
	int distY = y1 - y2;
	return distX*distX + distY*distY;
}

float ofxPolygonMask::distToCentre(ofVec3f a, ofVec3f b, ofVec3f p) {
	ofVec2f c((a.x+b.x)*0.5, (a.y+b.y)*0.5);
	float x = c.x - p.x;
	float y = c.y - p.y;
	return sqrt(x*x + y*y);
	
}

int ofxPolygonMask::insertPoint(ofVec3f p) {
	if(points.size()<3) {
		points.push_back(p);
		return points.size()-1;
	} else {
		float minDist = FLT_MAX;
		int minI = 0;
		for(int i = 0; i < points.size(); i++) {
			ofVec3f a = points[i];
			ofVec3f b = points[(i+1)%points.size()];
			float dist = distToCentre(a, b, p);
			if(dist<minDist) {
				minI = i;
				minDist = dist;
			}
		}
		int newIndex = ((minI+1)%points.size());
		points.insert(points.begin()+newIndex, p);
		return newIndex;
	}
}

void ofxPolygonMask::mousePressed(ofMouseEventArgs &m) {
	
	if(doPoint) {
		focusedPoint = insertPoint(ofVec3f(m.x-x, m.y-y, 60));
		selectedPoint = focusedPoint;
		return;
	} else if(doDelete) {
		for(int i = 0; i < points.size(); i++) {
			if(sqrDist(m.x-x, m.y-y, points[i].x, points[i].y)<SQR_DIST_POINT_GRAB) { // 5 pixels distance
				points.erase(points.begin()+i);
				return;
			}
		}
		
	}
	
	for(int i = 0; i < points.size(); i++) {
		if(sqrDist(m.x-x, m.y-y, points[i].x, points[i].y)<SQR_DIST_POINT_GRAB) { // 5 pixels distance
			selectedPoint = i;
			focusedPoint = i;
			//points[selectedPoint].x = m.x;
			//points[selectedPoint].y = m.y;

		}
	}
}

void ofxPolygonMask::mouseDragged(ofMouseEventArgs &m) {
	if(selectedPoint!=-1) {
		points[selectedPoint].x = m.x-x;
		points[selectedPoint].y = m.y-y;
		
		
	}
}

void ofxPolygonMask::mouseReleased(ofMouseEventArgs &m) {
	if(selectedPoint!=-1) {
		lastSelectedPoint = selectedPoint;
		selectedPoint = -1;
		save();
		
	}
}


void ofxPolygonMask::keyPressed(ofKeyEventArgs &k) {
	if(k.key=='\'') {
		doPoint = true;
	} else if(k.key=='\\') {
		doDelete = true;
	} else if(focusedPoint!=-1) {
		if(k.key=='-') {

			points[focusedPoint].z--;
			if(points[focusedPoint].z<0) points[focusedPoint].z = 0;

		} else if(k.key=='=') {

			points[focusedPoint].z++;
		
		} else if(k.key==OF_KEY_UP) {
			points[focusedPoint].y--;
		} else if(k.key==OF_KEY_DOWN) {
			points[focusedPoint].y++;
		} else if(k.key==OF_KEY_LEFT) {
			points[focusedPoint].x--;
		} else if(k.key==OF_KEY_RIGHT) {
			points[focusedPoint].x++;
		} else if(k.key==OF_KEY_BACKSPACE) {
			points.erase(points.begin()+focusedPoint);
			focusedPoint = -1;
		}
	}
}

void ofxPolygonMask::keyReleased(ofKeyEventArgs &k) {
	if(k.key=='\'') {
		doPoint = false;
	} else if(k.key=='\\') {
		doDelete = false;
	}
}



/**
 * Changes the direction of all the vertices. 
 * i.e. clockwise to anti-clockwise
 */
void ofxPolygonMask::flipDirection() {
	
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
void ofxPolygonMask::clear() {
	points.clear();
}



void ofxPolygonMask::toggleEditing() {
	setEnableEditing(!editing);
}

void ofxPolygonMask::setEnableEditing(bool editing) {
	if(this->editing!=editing) {
		this->editing = editing;
		if(this->editing) {
			// add events
			ofAddListener(ofEvents.keyPressed, this, &ofxPolygonMask::keyPressed);
			ofAddListener(ofEvents.keyReleased, this, &ofxPolygonMask::keyReleased);
			ofAddListener(ofEvents.mousePressed, this, &ofxPolygonMask::mousePressed);
			ofAddListener(ofEvents.mouseDragged, this, &ofxPolygonMask::mouseDragged);
			ofAddListener(ofEvents.mouseReleased, this, &ofxPolygonMask::mouseReleased);			
		} else {
			// remove events
			ofRemoveListener(ofEvents.keyPressed, this, &ofxPolygonMask::keyPressed);
			ofRemoveListener(ofEvents.keyReleased, this, &ofxPolygonMask::keyReleased);
			ofRemoveListener(ofEvents.mousePressed, this, &ofxPolygonMask::mousePressed);
			ofRemoveListener(ofEvents.mouseDragged, this, &ofxPolygonMask::mouseDragged);
			ofRemoveListener(ofEvents.mouseReleased, this, &ofxPolygonMask::mouseReleased);
		}
	}
}
bool ofxPolygonMask::isEditing() {
	return editing;
}


void ofxPolygonMask::toggleEnabled() {
	setEnabled(!enabled);
}

void ofxPolygonMask::setEnabled(bool enabled) {
	if(this->enabled!=enabled) {
		this->enabled = enabled;
		if(this->enabled) {
			// add events
			ofAddListener(ofEvents.draw, this, &ofxPolygonMask::draw);
		} else {
			// remove events
			ofRemoveListener(ofEvents.draw, this, &ofxPolygonMask::draw);
		}
	}
}
bool ofxPolygonMask::isEnabled() {
	return enabled;
}
