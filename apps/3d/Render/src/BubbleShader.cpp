/*
 *  BubbleShader.cpp
 *  Render
 *
 *  Created by Josh Nimoy on 3/6/11.
 ported from code by Joshua Noble
 *
 */

#include "BubbleShader.h"


BubbleShader::BubbleShader(){

	//ofDisableArbTex();
	
	//ofEnableNormalizedTexCoords();
	
	permImg.loadImage("shader/texturing.jpg");
	permImg.setImageType(OF_IMAGE_COLOR);
	glossImg.loadImage("shader/permutationTexture.jpg");
	glossImg.setImageType(OF_IMAGE_COLOR);
	
	//ofEnableArbTex();
	
	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);	
	
	sphereCenter = ofVec3f(0, 0, 0);
	cam.setTarget(sphereCenter);
	cam.setDistance(300);
	
	cubeshader.setup("shader/fresnel_refraction.vs", "shader/fresnel_refraction.fs");
	
	cubeMap.loadImages("shader/skybox/berkeley_positive_x.png",
					   "shader/skybox/berkeley_positive_y.png",
					   "shader/skybox/berkeley_positive_z.png",
					   "shader/skybox/berkeley_negative_x.png",
					   "shader/skybox/berkeley_negative_y.png",
					   "shader/skybox/berkeley_negative_z.png");
	
	//cubeMap.enableFixedMapping();
	
	xLightPos = -1;
	yLightPos = -1;
	zLightPos = 1;
	inc = true;
	//hasDrawnTex = false;

}

//------------------------------------------------------------------------------------------------

void BubbleShader::update(){
	
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
	
	//cout << xLightPos << " " << yLightPos << " " << zLightPos << endl;	
}

//------------------------------------------------------------------------------------------------

void BubbleShader::begin(){

	
	{
		glActiveTexture(GL_TEXTURE8);
		unsigned int texId1 = permImg.getTextureReference().getTextureData().textureID;
		unsigned int texTarget1 = permImg.getTextureReference().getTextureData().textureTarget;  
		glBindTexture(texTarget1, texId1);
		
		glActiveTexture(GL_TEXTURE9);
		unsigned int texId2 = glossImg.getTextureReference().getTextureData().textureID;
		unsigned int texTarget2 = glossImg.getTextureReference().getTextureData().textureTarget;  
		glBindTexture(texTarget2, texId2);
		
		glEnable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.textureObject);
	}		
	
	
	glEnableClientState(GL_NORMAL_ARRAY);

	
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	
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
	
	
	// marek bumped up the edge fall off - when I turned on GL_RGBA instead
	// of GL_RGB, everything looked a little dull (from 0.2f)
	cubeshader.setUniform1f("EdgeFalloff", 0.6f);
	
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
	
	//renderGluSphereWithTexCoords(900, 900, 100, 50);
	//renderGluSphereWithTexCoords(900, 1000, 200, 50);
	//renderGluSphereWithTexCoords(800, 800, 10, 50);
		
	
	

	
	delete cpos;
	delete pos;
	delete lPos;
	delete eyeVector;
	delete lightPosition;
	
}

//------------------------------------------------------------------------------------------------

void BubbleShader::end(){
	cubeshader.end();
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	
	//delete mat;
	
	glActiveTexture(GL_TEXTURE10);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	
	glActiveTexture(GL_TEXTURE9);
	glDisable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE8);
	glDisable(GL_TEXTURE_2D);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	
	//Reset texture 0
	glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	
	// global to content bubble now
	/*delete pos;
	 delete lPos;
	 delete eyeVector;
	 delete lpos;
	 */
	
	//glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTex);
	//glGetFloatv(GL_CURRENT_TEXTURE_COORDS, &currentCoords[0]);
	// cout << currentActiveTex << " " << currentCoords[0] << " " <<
	//	currentCoords[1] << " " << currentCoords[2] <<
	//	" " << currentCoords[3] << " " << endl;
	
}

//------------------------------------------------------------------------------------------------

void BubbleShader::renderGluSphereWithTexCoords(float x, float y, float z, float radius){
	glPushMatrix();
	
	glTranslatef(x, y, z);
	gluSphere(quadratic, radius, 50, 50);
	glPopMatrix();	
	
}

