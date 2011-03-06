#include "testApp.h"

//cube mapping tutorial found here:
//http://developer.nvidia.com/object/cube_map_ogl_tutorial.html

//--------------------------------------------------------------
void testApp::setup(){
	
	ofDisableArbTex();
	
	//cout << glGetString(GL_EXTENSIONS) << endl;
	ofEnableNormalizedTexCoords();
	
	permImg.loadImage("texturing.jpg");
	permImg.setImageType(OF_IMAGE_COLOR);
	glossImg.loadImage("permutationTexture.jpg");
	glossImg.setImageType(OF_IMAGE_COLOR);
	
	ofEnableArbTex();
	
	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);	
	
	sphereCenter = ofVec3f(0, 0, 0);
	cam.setTarget(sphereCenter);
	cam.setDistance(300);
	
	cubeshader.setup("fresnel_refraction.vs", "fresnel_refraction.fs");
	
	cubeMap.loadImages("skybox/berkeley_positive_x.png",
					   "skybox/berkeley_positive_y.png",
					   "skybox/berkeley_positive_z.png",
					   "skybox/berkeley_negative_x.png",
					   "skybox/berkeley_negative_y.png",
					   "skybox/berkeley_negative_z.png");
	
	//cubeMap.enableFixedMapping();
	
	xLightPos = -1;
	yLightPos = -1;
	zLightPos = 1;
	inc = true;
	//hasDrawnTex = false;
}

//--------------------------------------------------------------
void testApp::update(){

	if(inc) {
		xLightPos += 0.1;
		yLightPos += 0.1;
		//zLightPos += 1;
	} else {
		xLightPos -= 0.1;
		yLightPos -= 0.1;
		//zLightPos -= 1;
	}
	
	if(xLightPos > 20) {
		inc = false;
	} else if (xLightPos < -20) {
		inc = true;
	}
	
	cout << xLightPos << " " << yLightPos << " " << zLightPos << endl;
}

//--------------------------------------------------------------
void testApp::draw(){
	
	glClearColor( 0.f, 0.f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	cam.begin();

	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	
	cubeshader.begin();
	
	glActiveTexture(GL_TEXTURE0);
	unsigned int texId1 = permImg.getTextureReference().getTextureData().textureID;
	unsigned int texTarget1 = permImg.getTextureReference().getTextureData().textureTarget;  
	glBindTexture(texTarget1, texId1);
	
	glActiveTexture(GL_TEXTURE1);
	unsigned int texId2 = permImg.getTextureReference().getTextureData().textureID;
	unsigned int texTarget2 = glossImg.getTextureReference().getTextureData().textureTarget;  
	glBindTexture(texTarget2, texId2);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMap.textureObject);
	
	cubeshader.setUniform1i("glossMap", 0);
	cubeshader.setUniform1i("baseMap", 1);
	cubeshader.setUniform1i("environmentMap", 2);
	
	cubeshader.setUniform1f("EdgeFalloff", 0.2f);

	float* eyeVector = new float[3];
	eyeVector[0] = ofGetWidth()/2;
	eyeVector[1] = ofGetHeight()/2;
	eyeVector[2] = 1;
	cubeshader.setUniform3fv("eyeVector", eyeVector);
	
	float* lightPosition = new float[3];
	lightPosition[0] = xLightPos;
	lightPosition[1] = yLightPos;
	lightPosition[2] = zLightPos;
	cubeshader.setUniform3fv("lightVector", lightPosition);
	
	cubeshader.setUniform1f("reflectAmount", 0.6f);
	
	float* pos = new float[3];
	pos[0] = 0.5;
	pos[1] = 0.5;
	pos[2] = 0.5;
	cubeshader.setUniform3fv("fresnelValues", pos);
	
	float* cpos = new float[3];
	cpos[0] = modelview[3];//0.5;
	cpos[1] = modelview[7];//0.5;
	cpos[2] = modelview[11];//0.5;
	cubeshader.setUniform3fv("CameraPos", cpos);
	
	float* lPos = new float[3];
	lPos[0] = 0.5;
	lPos[1] = 0.5;
	lPos[2] = 0.5;
	cubeshader.setUniform3fv("IoR_Values", lPos);
	
	//cubeshader.setUniform4mat("ModelWorld4x4", mat);
	cubeshader.setUniform4mat("ModelWorld4x4", &modelview[0]);
	
	renderGluSphereWithTexCoords(10, 0, -100, 40);
	renderGluSphereWithTexCoords(10, 30, -200, 40);
	renderGluSphereWithTexCoords(20, 40, -10, 40);
	
	cubeshader.end();
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	
	delete pos;
	delete lPos;
	delete eyeVector;
	delete lightPosition;
	//delete mat;

	cam.end();


}

void testApp::renderGluSphereWithTexCoords(float x, float y, float z, float radius)
{
	glPushMatrix();
	
	glTranslatef(x, y, z);
	gluSphere(quadratic, radius, 50, 50);
	glPopMatrix();
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

