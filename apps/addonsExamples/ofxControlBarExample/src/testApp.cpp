#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	//setting up the control bar
	label1 = new ofxControlBar::Label("Mode: Triangle");
	controlBar.children.push_back(label1);
	controlBar.children.push_back(new ofxControlBar::Button("Triangle"));
	controlBar.children.push_back(new ofxControlBar::Button("Square"));
	controlBar.children.push_back(new ofxControlBar::Button("Pentagon"));
	controlBar.children.push_back(new ofxControlBar::Button("Hexagon"));
	controlBar.children.push_back(new ofxControlBar::Button("Heptagon"));
	controlBar.children.push_back(new ofxControlBar::Button("Octagon"));
	controlBar.children.push_back(new ofxControlBar::Button("Enneagon"));
	controlBar.children.push_back(new ofxControlBar::Button("Decagon"));
	controlBar.doLayout();
	
	mode = 3;
	rotScalar = ofRandomuf();
}

//--------------------------------------------------------------
void testApp::update(){
	controlBar.update(mouseX,mouseY);
	
	//handling control bar events
	while(ofxControlBar::Control::eventQueue.size()>0){
		ofxControlBar::Event evt = ofxControlBar::Control::eventQueue.front();
		ofxControlBar::Control::eventQueue.pop_front();
		if(evt.obj->className=="Button"){
			ofxControlBar::Button *btn = (ofxControlBar::Button*)evt.obj;
			if(btn->text=="Triangle"){
				mode = 3;
			}else if(btn->text=="Square"){
				mode = 4;
			}else if(btn->text=="Pentagon"){
				mode = 5;
			}else if(btn->text=="Hexagon"){
				mode = 6;
			}else if(btn->text=="Heptagon"){
				mode = 7;
			}else if(btn->text=="Octagon"){
				mode = 8;
			}else if(btn->text=="Enneagon"){
				mode = 9;
			}else if(btn->text=="Decagon"){
				mode = 10;
			}
			//indicate the button pressed on the label.
			label1->text = "Mode: " + btn->text;
			rotScalar = ofRandomuf();

		}
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	

	ofBackground(0,0,0);
	
	glPushMatrix();
	ofTranslate(ofGetWidth()/2,ofGetHeight()/2,0);
	ofRotate(ofGetFrameNum()*(rotScalar-0.5),0,0,1);
	ofSetColor(255,255,255);
	
	float rad = 200;
	int steps = mode;
	float inc = ((float)TWO_PI)/steps;
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0;i<steps;i++){
		float x = rad * cos(i*inc);
		float y = rad * sin(i*inc);
		glVertex2f(x,y);
	}
	glEnd();
	glPopMatrix();
	
	controlBar.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	controlBar.mouseDown();

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	controlBar.mouseUp();

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

