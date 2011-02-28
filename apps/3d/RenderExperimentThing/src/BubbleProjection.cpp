/**
 * BubbleProjection.cpp
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#include "BubbleProjection.h"

BubbleProjection::BubbleProjection() {
	interactiveArea = ofRectangle(100,100,900,500);
}

void BubbleProjection::setup() {
	ofDisableArbTex();
	cubeMap.loadImages("skybox/berkeley_positive_x.png",
					   "skybox/berkeley_positive_y.png",
					   "skybox/berkeley_positive_z.png",
					   "skybox/berkeley_negative_x.png",
					   "skybox/berkeley_negative_y.png",
					   "skybox/berkeley_negative_z.png");
	
	//cubeMap.enableFixedMapping();
	
	
	//first.setup(512, 512, GL_RGBA);
	//second.setup(512, 512, GL_RGBA);
	
	permTex.loadImage("Permutation Texture.png");
	glossTex.loadImage("texturing.png");
	
	sphereCenter = ofVec3f(0, 0, 0);
	cam.setTarget(sphereCenter);
	cam.setDistance(300);
	
	cubeshader.setup("fresnel_refraction.vs", "fresnel_refraction.fs");
	
	cubeshader.begin();
	cubeshader.setUniform1i("glossMap", 1);
	cubeshader.setUniform1i("baseMap", 2);
	cubeshader.end();
	
	hasDrawnTex = false;
	
	ofEnableArbTex();
		
}
















































//--------------------------------------------------------------
void BubbleProjection::update() {
}


//--------------------------------------------------------------
void BubbleProjection::draw() {
	
	ofDisableArbTex();
	ofClear(0, 80, 0, 50);
	
	//ofBackground(0);
	
	//glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
		
	cam.begin();
	
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	
	cubeshader.begin();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.textureObject);
	cubeshader.setUniform1i("environmentMap", 0);
	
	/*
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, (GLuint)glossTex.getTextureReference().texData.textureID);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	
	
	//first.getTexture(0).bind();
	
	glActiveTexture(GL_TEXTURE2);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, (GLuint)permTex.getTextureReference().texData.textureID);
	//second.getTexture(0).bind();
	*/
	cubeshader.setUniform1i("glossMap", 1);
	cubeshader.setUniform1i("baseMap", 2);
	
	
	cubeshader.setUniform1f("EdgeFalloff", 0.02);
	
	float* pos = new float[3];
	pos[0] = 0.3;
	pos[1] = 0.3;
	pos[2] = 0.3;
	cubeshader.setUniform3fv("fresnelValues", pos);
	
	float* cpos = new float[3];
	cpos[0] = 0.107;
	cpos[1] = 0.0;
	cpos[2] = 1.f;
	cubeshader.setUniform3fv("CameraPos", cpos);
	
	float* lPos = new float[3];
	lPos[0] = 0.7;
	lPos[1] = 0.0;
	lPos[2] = 0.9;
	cubeshader.setUniform3fv("IoR_Values", lPos);
	
	float* mat = new float[16];
	mat[0] = 0.21;
	mat[1] = 0.715; 
	mat[2] = 0.75; 
	mat[3] = 0.75;
	mat[4] = 0; 
	mat[5] = 1; 
	mat[6] = 0; 
	mat[7] = 0; 
	mat[8] = 0; 
	mat[9] = 0; 
	mat[10] = 1; 
	mat[11] = 0; 
	mat[12] = 0; 
	mat[13] = 0;
	mat[14] = 0.43;
	mat[15] = 1;
	
	cubeshader.setUniform4mat("ModelWorld4x4", mat);
	
	ofEnableAlphaBlending();
	
	ofSphere(10, 0, -100, 40);
	ofSphere(10, 30, -200, 40);
	ofSphere(20, 40, -10, 40);
	
	
	//glPopAttrib();
	
	
	
	
	delete pos;
	delete lPos;
	delete mat;
	
	
	cubeshader.end();
	glDisable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_TEXTURE_2D);
	cam.end();
	
	ofEnableArbTex();
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	
}














ofRectangle &BubbleProjection::getInteractiveArea() {
	return interactiveArea;
}

void BubbleProjection::touchDown(float x, float y, int touchId) {
	

	ofVec2f touchCoords(x, y);
	
	// find the closest point to the new touch
	float minSqrDist = FLT_MAX; // do squares
	int minTouchId = -1;

	for(tIt = touches.begin(); tIt!=touches.end(); tIt++) {
		float sqrDist = touchCoords.squareDistance((*tIt).second);
		if(sqrDist<minSqrDist) {
			minTouchId = (*tIt).first;
			minSqrDist = sqrDist;
		}
	}
	
	// the minimum distance between the 2 closest touches 
	// in order for it to be a double touch.
	float doubleTouchDist = 0.1;
	
	// add the touch
	touches[touchId] = touchCoords;
	// if there's another touch, and it's close enough, call doubleTouchGesture
	if(minTouchId!=-1 && sqrt(minSqrDist)<doubleTouchDist) { 
		doubleTouchGesture(touchId, minTouchId);
		
	}
	
}
void BubbleProjection::touchMoved(float x, float y, int touchId) {
	touches[touchId] = ofVec2f(x, y);
}

void BubbleProjection::touchUp(float x, float y, int touchId) {
	if(touches.find(touchId)!=touches.end()) {
		touches.erase(touchId);
	}
}

ofVec2f BubbleProjection::mapToInteractiveArea(ofVec2f inPoint) {
	return ofVec2f(interactiveArea.x + interactiveArea.width * inPoint.x,
				   interactiveArea.y + interactiveArea.height * inPoint.y);
}

void BubbleProjection::doubleTouchGesture(int touch1Id, int touch2Id) {
	

	ofVec2f doubleTouchCentre = (touches[touch1Id] + touches[touch2Id])/2;
	printf("doubleTouchGesture\n");
}
