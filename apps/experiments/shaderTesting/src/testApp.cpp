#include "testApp.h"

//cube mapping tutorial found here:
//http://developer.nvidia.com/object/cube_map_ogl_tutorial.html

//--------------------------------------------------------------
void testApp::setup(){
	
	
	ofDisableArbTex();
	
	/*cubeMap.loadImages("pos_x.png",
					   "pos_y.png",
					   "pos_z.png",
					   "neg_x.png",
					   "neg_y.png",
					   "neg_z.png"); */
	
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
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	//ofBackground(0);
	
	//glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	
	/*if( !hasDrawnTex )
	{
		first.begin();
		permTex.draw(0, 0, 0);
		first.end();
		second.begin();
		glossTex.draw(0, 0, 0);
		second.end();
		hasDrawnTex = true;
	}*/
	
	cam.begin();
	
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
		
	cubeshader.begin();
	
	//cubeMap.enableFixedMapping();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.textureObject);
	cubeshader.setUniform1i("environmentMap", 0);
	
	//cubeMap.disableFixedMapping();
	
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, (GLuint)glossTex.getTextureReference().texData.textureID);
	//first.getTexture(0).bind();

	glActiveTexture(GL_TEXTURE2);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, (GLuint)permTex.getTextureReference().texData.textureID);
	//second.getTexture(0).bind();

	cubeshader.setUniform1i("glossMap", 1);
	cubeshader.setUniform1i("baseMap", 2);
	

	cubeshader.setUniform1f("EdgeFalloff", 0.2);
	
	float* pos = new float[3];
	pos[0] = 0.8;
	pos[1] = 0.8;
	pos[2] = 0.8;
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
	
	glDisable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_CUBE_MAP);
	//glPopAttrib();

	glDisable(GL_TEXTURE_2D);
	
	cubeshader.end();
	
	delete pos;
	delete lPos;
	delete mat;
	
	cam.end();

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
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

