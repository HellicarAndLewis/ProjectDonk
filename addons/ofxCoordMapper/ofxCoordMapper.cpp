/**
 * ofxCoordMapper.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 18/02/2011.
 *
 */

#include "ofxCoordMapper.h"
ofxCoordMapper::ofxCoordMapper() {
	lut = NULL;
	
}

ofxCoordMapper::~ofxCoordMapper() {
	if(lut!=NULL) delete [] lut;
}
void ofxCoordMapper::setup() {
	width = 640;
	height = 480;
	ofEnableArbTex();
	coordColors.setup(256,256, GL_RGB32F);
	drawColorsIntoTexture();
	lutFbo.setup(width, height, GL_RGB32F);
	
	lut = new float[width*height*3];
	debugSaved.allocate(width, height, OF_IMAGE_COLOR);

}

void ofxCoordMapper::drawLUT() {
	coordColors.draw(0, 0);
}

void ofxCoordMapper::drawColorsIntoTexture() {
	
	coordColors.begin();
	ofClear(0, 0, 0, 0);
	
	
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(0, 0);
	
	glColor3f(1, 0, 0);
	glVertex2f(coordColors.getWidth(), 0);
	
	glColor3f(1, 1, 0);
	glVertex2f(coordColors.getWidth(), coordColors.getHeight());

	glColor3f(0, 1, 0);
	glVertex2f(0, coordColors.getHeight());
	
	glEnd();
	
	coordColors.end();
	
	glColor3f(1,1,1);
}

void ofxCoordMapper::beginCapture() {
	/*lutFbo.begin();
	ofClear(0,0,0,0);
	 */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(coordColors.getWidth(), coordColors.getHeight(), 1);
	
	glMatrixMode(GL_MODELVIEW);
	
	glColor3f(1, 1, 1);
	coordColors.getTexture(0).bind();
}

ofVec2f ofxCoordMapper::lookUp(ofVec2f inPos) {
	// scale to our lut
	inPos *= ofVec2f(width, height);
	
	// check we're in bounds.
	if(inPos.x<0 || inPos.y<0 || inPos.x>width-1 || inPos.y>height-1) return ofVec2f(0,0);
	
	// find that pixel.
	int inX = inPos.x;
	int inY = inPos.y;
	int offset = (inX + inY*width)*3;
	return ofVec2f(lut[offset], lut[offset+1]);
}


void ofxCoordMapper::endCapture() {
	coordColors.getTexture(0).unbind();
	/*
	
	
	// capture pixels
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, lut);
	
	unsigned char *c = new unsigned char[width*height*3];
	for(int i = 0; i < width*height*3; i++) {
		c[i] = lut[i]*255.f;
	}

    debugSaved.setFromPixels(c, width, height, OF_IMAGE_COLOR);
    debugSaved.saveImage("lut.png");
	delete [] c;	
	
	
	lutFbo.end();
	
	glColor3f(1,1,1);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	

	lutFbo.draw(0,0);
	//coordColors.draw(0,0);
	// now get the pixels out of the fbo
	 */
}

ofFbo *ofxCoordMapper::getCoordTexture() {
	return &coordColors;
}
