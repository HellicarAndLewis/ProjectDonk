/*
 *  BubbleData.cpp
 *  Render
 *
 *  Created by Josh Nimoy on 2/11/11.
 *
 */

#include "BubbleData.h"
#include <GLUT/GLUT.h>
#include "BubbleProjection.h"

namespace Donk{

	ofTrueTypeFont BubbleData::font;
	vector<BubbleData*> BubbleData::all;

	void BubbleData::add(ofxOscMessage &m){
		all.push_back(new BubbleData(m));
	}
	
	void BubbleData::render(){
		//draw all the bubbles
		glPushMatrix();
		vector<BubbleData*>::iterator bdit;
		for(bdit=all.begin();bdit!=all.end();bdit++)(*bdit)->draw();
		glPopMatrix();
		
	}
	
	
	void BubbleData::draw(){
		if(profileImage.width != 0){
			ofSetColor(255,255,255);
			profileImage.bind();
			float w = profileImage.width;
			float h = profileImage.height;
			
			//draw the circle-masked thumbnail
			int steps = 60;
			float inc = TWO_PI/(float)steps;			
			glBegin(GL_TRIANGLE_FAN);
			for(int i=0;i<steps;i++){
				float x = cos(inc*i);
				float y = sin(inc*i);
				glTexCoord2f((x+1)*w*0.5,(y+1)*h*0.5);
				glVertex2f(x*radius,y*radius);
			}
			glEnd();
			profileImage.unbind();
			
			if(!font.bLoadedOk){
				font.loadFont("global/font/Gotham-Bold.otf",50);
			}
			
			ofRectangle textBB = font.getStringBoundingBox(userName, 0,0);
			glPushMatrix();
			float s = radius/textBB.width*1.75;
			glScalef(s,s,s);
			glTranslated(-textBB.width/2, 0,0.2);
			ofSetColor(0,0,0);
			font.drawString(userName,0,0);
			glTranslatef(2,2,0.2);
			ofSetColor(255,255,255);
			font.drawString(userName,0,0);
			//profileImage.unbind();
			
			renderSphere();
			
			glPopMatrix();
		}
		
	}
	
	void BubbleData::renderSphere()
	{
		
		
		GLint currentActiveTex;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTex);
		float currentCoords[4];
		glGetFloatv(GL_CURRENT_TEXTURE_COORDS, &currentCoords[0]);
		
		cout << currentActiveTex << " " << currentCoords[0] << " " << currentCoords[1] << " " << currentCoords[2] << " " << currentCoords[3] << " " << endl;
		
		glEnableClientState(GL_NORMAL_ARRAY);
		
		glEnable(GL_NORMALIZE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		
		float modelview[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
		
		shader.begin();
		
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
		
		shader.setUniform1i("glossMap", 1);
		shader.setUniform1i("baseMap", 2);
		shader.setUniform1i("environmentMap", 3);
		
		shader.setUniform1f("EdgeFalloff", 0.2f);
		
		float* eyeVector = new float[3];
		eyeVector[0] = ofGetWidth()/2;
		eyeVector[1] = ofGetHeight()/2;
		eyeVector[2] = 1;
		shader.setUniform3fv("eyeVector", eyeVector);
		
		float* lpos = new float[3];
		lpos[0] = lightPosition.x;
		lpos[1] = lightPosition.y;
		lpos[2] = lightPosition.z;
		shader.setUniform3fv("lightVector", lpos);
		
		shader.setUniform1f("reflectAmount", 0.6f);
		
		float* pos = new float[3];
		pos[0] = 0.5;
		pos[1] = 0.5;
		pos[2] = 0.5;
		shader.setUniform3fv("fresnelValues", pos);
		
		float* cpos = new float[3];
		cpos[0] = modelview[3];//0.5;
		cpos[1] = modelview[7];//0.5;
		cpos[2] = modelview[11];//0.5;
		shader.setUniform3fv("CameraPos", cpos);
		
		float* lPos = new float[3];
		lPos[0] = 0.5;
		lPos[1] = 0.5;
		lPos[2] = 0.5;
		shader.setUniform3fv("IoR_Values", lPos);
		
		shader.setUniform4mat("ModelWorld4x4", &modelview[0]);
		//gluSphere(quadratic, rigidBody->boxSize.getX(), rigidBody->boxSize.getX(), rigidBody->boxSize.getX());
		gluSphere(quadratic, 100, 20, 20);
		
		shader.end();
		
		glDisable(GL_CULL_FACE);
		glDisable(GL_NORMALIZE);
		glDisable(GL_DEPTH_TEST);
		
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
		
		delete pos;
		delete lPos;
		delete eyeVector;
		delete lpos;
		 
		
		glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTex);
		glGetFloatv(GL_CURRENT_TEXTURE_COORDS, &currentCoords[0]);
		cout << currentActiveTex << " " << currentCoords[0] << " " << currentCoords[1] << " " << currentCoords[2] << " " << currentCoords[3] << " " << endl;
		
		
	}
	
	
	void BubbleData::update(){
		//call update on all the bubbles
		vector<BubbleData*>::iterator bdit;
		for(bdit=all.begin();bdit!=all.end();bdit++){
			if(!(*bdit)->doneLoading()) {
				(*bdit)->step();
			}
		}
		
	}

	
	
	void BubbleData::step(){
		
		if(profileImageLoader!=NULL){
			if(profileImageLoader->status==2){
				ofBuffer buff;
				buff.set(profileImageLoader->data.c_str(), profileImageLoader->data.size());
				profileImage.loadImage(buff);
				delete profileImageLoader;
				profileImageLoader = NULL;
			}
		}
		
		vector<MediaEntry>::iterator mit;
		bool allMediaLoaded = true;
		for(mit=media.begin();mit!=media.end();mit++){
			mit->step();
			
			// check to see if we're totally loaded
			if(mit->thumbImage.width==0 && mit->mediaImage.width==0) {
				allMediaLoaded = false;
			}
		}
		
		bool oldLoadingDone = loadingDone;
		loadingDone = profileImage.width>0 && allMediaLoaded;
		
		if(!oldLoadingDone && loadingDone) {
			// put it in the physics system
			printf("Done loading %s %s\n", userName.c_str(), text.c_str());
			BubbleProjection::getInstance()->bubbleReceived(this);
		}
		
		
	}
	
	BubbleData::BubbleData(ofxOscMessage &m){
		
		loadingDone = false;
		radius = ofRandomuf()*100+50;
		
		int index=0;
		profileImageLoader = NULL;
		
		while(index<m.getNumArgs()){
			string key = m.getArgAsString(index);
			index++;
			if(key=="mode"){
				mode = m.getArgAsString(index);
			}else if(key=="queueID"){
				id = m.getArgAsString(index);
			}else if(key=="profileImageURL"){
				profileImageURL = m.getArgAsString(index);
				
				//start downloading it
				profileImageLoader = new AsyncHttpLoader();
				profileImageLoader->get( profileImageURL, "" );
				
			}else if(key=="userName"){
				userName = m.getArgAsString(index);
			}else if(key=="text"){
				text = m.getArgAsString(index);
			}else if(key=="questionID"){
				questionID = m.getArgAsString(index);
			}else if(key=="text"){
				tag = m.getArgAsString(index);
			}else if(key=="mediaID"){
				media.push_back(MediaEntry());
				media.back().id = m.getArgAsString(index);
			}else if(key=="mediaThumbURL"){
				media.back().thumb_url = m.getArgAsString(index);
				//start downloading it
				media.back().thumbLoader = new AsyncHttpLoader();
				media.back().thumbLoader->get( media.back().thumb_url, "" );
				
			}else if(key=="mediaURL"){
				media.back().url = m.getArgAsString(index);
				media.back().mediaLoader = new AsyncHttpLoader();
				media.back().mediaLoader->get( media.back().url, "" );
			}
			index++;
		}
		
		// try setting up the gl stuff
		
		ofDisableArbTex();
		//cout << glGetString(GL_EXTENSIONS) << endl;
		//ofEnableNormalizedTexCoords();
		
		permImg.loadImage("shader/texturing.jpg");
		permImg.setImageType(OF_IMAGE_COLOR);
		glossImg.loadImage("shader/permutationTexture.jpg");
		glossImg.setImageType(OF_IMAGE_COLOR);
		
		ofEnableArbTex();
		
		quadratic = gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
		gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
		gluQuadricTexture(quadratic, GL_TRUE);	
		
		shader.setup("shader/fresnel_refraction.vs", "shader/fresnel_refraction.fs");
		
		cubeMap.loadImages("shader/skybox/berkeley_positive_x.png",
						   "shader/skybox/berkeley_positive_y.png",
						   "shader/skybox/berkeley_positive_z.png",
						   "shader/skybox/berkeley_negative_x.png",
						   "shader/skybox/berkeley_negative_y.png",
						   "shader/skybox/berkeley_negative_z.png");
		
		
		
	}

	BubbleData::~BubbleData(){
		if(profileImageLoader!=NULL)delete profileImageLoader;
	}
	
	BubbleData::MediaEntry::MediaEntry(){
		thumbLoader = NULL;
		mediaLoader = NULL;
	}
	
	BubbleData::MediaEntry::~MediaEntry(){
		if(thumbLoader!=NULL)delete thumbLoader;
		if(mediaLoader!=NULL)delete mediaLoader;
	}
	
	void BubbleData::MediaEntry::step(){
		
		if(thumbLoader!=NULL){
			if(thumbLoader->status==2){
				ofBuffer buff;
				buff.set(thumbLoader->data.c_str(), thumbLoader->data.size());
				thumbImage.loadImage(buff);
				delete thumbLoader;
				thumbLoader = NULL;
			}
		}

		if(mediaLoader!=NULL){
			if(mediaLoader->status==2){
				ofBuffer buff;
				buff.set(mediaLoader->data.c_str(), mediaLoader->data.size());
				mediaImage.loadImage(buff);
				delete mediaLoader;
				mediaLoader = NULL;
			}
		}
		
	}
	
	
	bool BubbleData::doneLoading() {
		return loadingDone;
	}
}
