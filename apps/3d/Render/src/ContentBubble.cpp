/*
 *  ContentBubble.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 2/26/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ContentBubble.h"

ofTrueTypeFont ContentBubble::font;

//--------------------------------------------------------------
void ContentBubble::destroy() {
	if(rigidBody) {
		rigidBody->destroy();
		rigidBody = NULL;
	}
}

//--------------------------------------------------------------
ContentBubble::ContentBubble() {
	
	timeSinceLastFlip = 0;
	bFlipToggle      = false;
	bRemoveOffScreen = false;
	bRemove	         = false;
	bAnimateOut		 = false;
	bAnimateIn		 = false;
	rigidBody		 = NULL;
	bTouched		 = false;
	bDoubleTouched	 = false;
	rotateY			 = 0;
	touchID			 = -1;
	distanceToTarget = 10000; // so i dont call sqrt() so much.
	rotateYTarget	 = 0;
	targetForce		= 20.f;
	
	if(ofRandomuf()>0.5){
		rotateYDirectionOn = 180;
	}else{
		rotateYDirectionOn = -180;
	}
	
	rotateYDirectionOff = 0;
	
	alpha = 255;
	
	bAlive = false;
	
	// for now!
	offScreenTaget.y = -100;
	offScreenTaget.z = -200;
	
	bobTheta.x = ofRandomuf() * PI;
	bobTheta.y = ofRandomuf() * PI;
	bobTheta.z = ofRandomuf() * PI;
	
	buzzTime	= 0;
	buzzWait	= 0.1;
	loopCounter = 0;
    loopTime = ofRandom(.0015/2.0,.0025/2.0);
	
    maxFontSize = 15;
    minFontSize = 9;
	
	zoomTouched	= false;


}

//--------------------------------------------------------------
void ContentBubble::createContentBubble() {
	
	color.r = ofRandom(0, 255);
	color.g = ofRandom(0, 255);
	color.b = ofRandom(0, 255);
	color.a = 25;
	
	touchAlphaTarget = 0;
	touchAlpha		 = 0;
	
	for (int i=0; i<16; i++) {
		billboadMatrix[i] = 0;
		m[i]			  = 0;
	}
	
	bAlive = true;
	birthDate = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------
void ContentBubble::setContentSideUp()
{
	if(ofRandomuf()>0.5){
		rotateYDirectionOff = 180;
	}else{
		rotateYDirectionOff = -180;
	}
	
	rotateYDirectionOn = 0;
	rotateYTarget = rotateYDirectionOff;
	rotateY = rotateYDirectionOff;
}

//--------------------------------------------------------------
void ContentBubble::setUserSizeUp()
{
	if(ofRandomuf()>0.5){
		rotateYDirectionOn = 180;
	}else{
		rotateYDirectionOn = -180;
	}
	
	rotateYDirectionOff = 0;
	rotateYTarget = rotateYDirectionOff;
	rotateY = rotateYDirectionOff;
}

//--------------------------------------------------------------
void ContentBubble::gotoTarget(float scale) {
	if(rigidBody->isBody()) {
		
		rigidBody->body->setDamping(0.99, 0.99); // <-- add some crazy damping
		
		ofVec3f frc = target - rigidBody->getBulletPosition();
		distanceToTarget = frc.length();
		float d = ABS(distanceToTarget) * scale;
		d *= targetForce;
		frc.normalize();
		frc *= d;
		rigidBody->body->clearForces();
		rigidBody->body->applyCentralForce(btVector3(frc.x, frc.y, frc.z));
		
		// printf("taget\n");
	}
}

//--------------------------------------------------------------
void ContentBubble::bobMe() {

	bobTheta += 0.02;
	
	float amp = 100;
	target.x = resetTarget.x + cos(	bobTheta.x ) * 10.0;
	target.y = resetTarget.y + sin(	bobTheta.y ) * amp;
	target.z = resetTarget.z + sin(	bobTheta.z ) * 30.0;
}

//--------------------------------------------------------------
void ContentBubble::buzzMe() {
	
	// maybe there is a moe elegant solution...
	
	float tForce = 50.f;
	if(targetForce < tForce) 
		targetForce = targetForce + (tForce-targetForce)*.1;
	
	float dt = ofGetElapsedTimef() - buzzTime;
	
	if(dt > buzzWait)
	{
		buzzTime = ofGetElapsedTimef();
		buzzWait = ofRandom(.1,.5);
		
		float rads = ofRandom(60,80);
		float angle = ofRandom(0,TWO_PI);
		buzzDest.x = resetTarget.x + rads * cos(angle);
		buzzDest.y = resetTarget.y + rads * sin(angle);
				
		buzzOrig = getPosition();
	}
	
	float pct = 1 - powf( (dt / buzzWait),.75);
	
	target.x = pct * buzzOrig.x + (1-pct) * buzzDest.x;
	target.y = pct * buzzOrig.y + (1-pct) * buzzDest.y;

	
}

//--------------------------------------------------------------
void ContentBubble::loopMe(float interactiveWidth,float interactiveHeight){
	
	if(loopCounter < 1 ) loopCounter += loopTime;//.0015;
	
	if( getPosition().y < -radius )
	{
		loopCounter = 0;
		rigidBody->setPosition(ofVec3f( (interactiveWidth/2.f) + ofRandom(-300, 300), interactiveHeight+radius, getPosition().z ), ofVec3f(0,0,0), 0);
	}
	
	target.x = getPosition().x;
	target.y = ofLerp(-radius,interactiveHeight,1-loopCounter);//-radius,loopCounter);
}

//--------------------------------------------------------------
void ContentBubble::setLoopStart(float interactiveHeight){
	
	float scl = interactiveHeight / (interactiveHeight+radius);
	loopCounter = 1 - (getPosition().y / interactiveHeight)*scl;
}

//--------------------------------------------------------------
void ContentBubble::setTarget(float x, float y, float z) {
	target = ofVec3f(x, y, z);
	resetTarget = ofVec3f(x, y, z);
}

//--------------------------------------------------------------
void ContentBubble::setTarget(ofVec3f v) {
	setTarget(v.x, v.y, v.z);
}

//--------------------------------------------------------------
ofVec3f ContentBubble::getPosition() {
	if(rigidBody->isBody()) {
		return rigidBody->getPosition();
	}
}

//--------------------------------------------------------------
void ContentBubble::addForce(float x, float y, float z, float scale) {
	if(rigidBody->isBody()) {
		
		// rigidBody->body->setDamping(0.99, 0.99); // <-- add some crazy damping
		
		ofVec3f frc = ofVec3f(x, y, z) * scale;
		rigidBody->body->clearForces();
		rigidBody->body->applyCentralForce(btVector3(frc.x, frc.y, frc.z));
		
	}	
}

//--------------------------------------------------------------
void ContentBubble::goOffScreen() {
	if(rigidBody->isBody()) {
		
		rigidBody->body->setDamping(0.99, 0.99); // <-- add some crazy damping
		
		ofVec3f frc = offScreenTaget - rigidBody->getBulletPosition();
		distanceToTarget = frc.length();
		float d = ABS(distanceToTarget);
		d *= 20.0;
		frc.normalize();
		frc *= d;
		
		rigidBody->body->clearForces();
		rigidBody->body->applyCentralForce(btVector3(frc.x, frc.y, frc.z));
		
	}
}


//--------------------------------------------------------------
void ContentBubble::addAtrractionForce(float x, float y, float z, float scale) {
	if(rigidBody->isBody()) {
		
		ofVec3f frc = ofVec3f(x, y, z) - rigidBody->getBulletPosition();
		distanceToTarget = frc.length();
		float d = ABS(distanceToTarget);
		d *= scale;
		frc.normalize();
		frc *= d;
		
		rigidBody->body->clearForces();
		rigidBody->body->applyCentralForce(btVector3(frc.x, frc.y, frc.z));
		
	}
}

//--------------------------------------------------------------
void ContentBubble::addAtrractionForce(ofVec3f &p, float scale) {
	addAtrractionForce(p.x, p.y, p.z, scale);	
}

//--------------------------------------------------------------
void ContentBubble::doubleTouched() {
	bDoubleTouched = true;	
	
	if(bDoubleTouched) {
		
		float time = (ofGetElapsedTimeMillis()-timeSinceLastFlip) / 1000.0;
		if(time < 2.0) {
			return;
		}

		bFlipToggle = !bFlipToggle;
		touchAlphaTarget = 160;
		
		if(bFlipToggle) {
			rotateYTarget = rotateYDirectionOn;
			timeSinceLastFlip = ofGetElapsedTimeMillis();
		}
		else {
			rotateYTarget = rotateYDirectionOff;
			timeSinceLastFlip = ofGetElapsedTimeMillis();
		}
		
	}
	
	
}

//--------------------------------------------------------------
void ContentBubble::releaseTouch() {
	
	if(bDoubleTouched) {
		printf("double Touch Relesed\n");	
	}
	touchAlphaTarget = 0;
}


//--------------------------------------------------------------
void ContentBubble::setRadius(float r) {
	
	btSphereShape * sphereShape = (btSphereShape*)rigidBody->body->getCollisionShape();
	sphereShape->setUnscaledRadius(r);
	radius = r;
	
}

//--------------------------------------------------------------
void ContentBubble::lerpRadius(float r,float speed) {
	setRadius(radius + (r-radius)*speed);	
}

//--------------------------------------------------------------
void ContentBubble::update() {
	
	//lazyload the font
    
    /*
	if(!font.bLoadedOk){
		font.loadFont("global/font/Gotham-Bold.otf", 50);
		if(font.bLoadedOk) {
			printf("--- font is loaded ---\n");	
		}
	}
	*/
    

	touchAlpha += (touchAlphaTarget-touchAlpha) * 0.1;
	rotateY += (rotateYTarget-rotateY) * 0.05;
	age = ofGetElapsedTimeMillis()-birthDate;
	
	
	if(rigidBody->isBody()) {
		
		pos = rigidBody->getPosition();
		rigidBody->body->getWorldTransform().getOpenGLMatrix(m);		
		btSphereShape * sphereShape = (btSphereShape*)rigidBody->shape;
		
		//radius doesn't include the margin, so draw with margin
		radius = sphereShape->getMargin();
		
		// create the billboard matrix
		for (int i=0; i<16; i++) {
			billboadMatrix[i] = m[i];
		}
		
		// ripped from here
		// http://lighthouse3d.com/opengl/billboarding/index.php?billCheat
		for(int i=0; i<3; i++ ) {
			for(int j=0; j<3; j++ ) {
				if ( i==j)  billboadMatrix[i*4+j] = 1.0;
				else		billboadMatrix[i*4+j] = 0.0;
			}
		}
	}
}

//--------------------------------------------------------------
void ContentBubble::pushBubble() {
	
	if(rigidBody->isBody()) {
	
		glPushMatrix();
		glMultMatrixf(m);
		
	}
	
}

//--------------------------------------------------------------
void ContentBubble::popBubble() {
	if(rigidBody->isBody()) {
		glPopMatrix();
	}
	
}


//--------------------------------------------------------------
void ContentBubble::pushBillboard() {
	glPushMatrix();
	glMultMatrixf(billboadMatrix);
}

//--------------------------------------------------------------
void ContentBubble::popBillboard() {
	glPopMatrix();
}

void ContentBubble::loadFont()
{
    //lazyload the font
    if(!textDisplay.bFontsLoaded){

        textDisplay.setup("global/font/Gotham-Bold.otf", minFontSize, maxFontSize, 1, maxRadius); 
        
        string txt = data != NULL ? data->text : "nothing";
        if(txt.empty())txt="lorem ipsum"; //hmmm....maybe better answer?
        
        textDisplay.setMessage( txt );        
    }
    
   	if(!font.bLoadedOk){
		font.loadFont("global/font/Gotham-Bold.otf", 50);
		if(font.bLoadedOk) {
			printf("--- font is loaded ---\n");	
		}
	}
 
}

//--------------------------------------------------------------
void ContentBubble::drawTwitterData() {
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glMultMatrixf(billboadMatrix);
	
	glRotatef(rotateY,0,1,0);

	// the twitter icon and text
	if(data) {
		//if no twitter icon, don't draw.
		if(data->profileImage.width != 0){
			
			float data_radius = radius;//*0.9;
			            
            loadFont();
            
			{
				//draw twitter icon as a disk
				ofSetColor(255,255,255, alpha);
				data->profileImage.bind();
				float w = data->profileImage.width;
				float h = data->profileImage.height;
				//draw the circle-masked thumbnail
				int steps = 60;
				float inc = TWO_PI/(float)steps;
				glBegin(GL_TRIANGLE_FAN);
				for(int i=0;i<steps;i++){
					float x = cos(inc*i);
					float y = sin(inc*i);
					glTexCoord2f((x+1)*w*0.5,(y+1)*h*0.5);
					glVertex2f(x*data_radius,y*data_radius);
				}
				glEnd();
				data->profileImage.unbind();
			}
			
			{
				glPushMatrix();
				glRotatef(180,0,1,0);
				glTranslatef(0,0,0.1);
				
				//draw twitter media as a disk
				ofSetColor(color,alpha);
				if(data->media.size()){
					ofSetColor(255,255,255, alpha);
					data->media[0].mediaImage.bind();

					float w = data->media[0].mediaImage.width;
					float h = data->media[0].mediaImage.height;
					//draw the circle-masked thumbnail
					int steps = 60;
					float inc = TWO_PI/(float)steps;
					glBegin(GL_TRIANGLE_FAN);
					for(int i=0;i<steps;i++){
						float x = cos(inc*i);
						float y = sin(inc*i);
						glTexCoord2f((x+1)*w*0.5,(y+1)*h*0.5);
						glVertex2f(x*data_radius,y*data_radius);
					}
					glEnd();
					
					data->media[0].mediaImage.unbind();
				}
				glPopMatrix();
			}
			
			{
				//draw twitter text content
                string userName = data->userName;
				ofRectangle textBB = font.getStringBoundingBox(userName, 0,0);
				glPushMatrix();
				float s = data_radius/textBB.width*1.75;
                glScalef(s,s,s);
				glTranslated(-textBB.width/2, 0,0.2);

				ofSetColor(0,0,0, alpha);
				font.drawString(userName,0,0);
				glTranslatef(-2*s,-2*s,2);

				ofSetColor(255,255,255, alpha);
                font.drawString(userName,0,0);
				//data->profileImage.unbind();	 //if i take these out, the GUI doesn't draw?		//jg doesn't seem necessary anymore
				glPopMatrix();
                
				
//				if(txt.empty())txt="lorem ipsum";
//				ofRectangle textBB = font.getStringBoundingBox(txt, 0,0); //need to cope with up to 140 characters here and UTF strings...
//				glPushMatrix();
//				float s = data_radius/textBB.width*1.75;
//				glScalef(s,s,s);
//				glRotatef(180,0,1,0);
//				glTranslated(-textBB.width/2, 0,0.2);
//				ofSetColor(0,0,0, alpha);
//				font.drawString(txt,0,0);
//				glTranslatef(2,2,0.2);
//				ofSetColor(255,255,255, alpha);
//				font.drawString(txt,0,0);
//				data->profileImage.unbind();			
//				glPopMatrix(); //if i take these out, the GUI doesn't draw?		
                
                //data->profileImage.unbind();	 //if i take these out, the GUI doesn't draw?		
                
                //cout << "current radius is " << data_radius << " max radius is " << maxRadius << endl;
                
                //DRAW Twitter Text
                glPushMatrix();
                glRotatef(180,0,1,0); //always flip around or text is inverted
                glTranslatef(0,0,1);
                s = data_radius/maxRadius * 1.75;
                ofScale(s,s,s);
                ofSetColor(0,0,0, alpha);
                textDisplay.draw(0, 10);
                glTranslatef(-.75,-.75,1);
                ofSetColor(255,255,255, alpha);
                textDisplay.draw(0, 10);
                glPopMatrix();
               
                //JG debug square - leave in plz
//                ofPushStyle();
//                ofSetRectMode(OF_RECTMODE_CENTER);
//                ofNoFill();
//                ofSetColor(0,255,0, alpha);
//                ofRect(0, 0, data_radius*contentExpand.x, data_radius*contentExpand.y);
//                ofPopStyle();                
                
			}
		}
		
	}
	glPopMatrix();
}

//--------------------------------------------------------------
void ContentBubble::drawHighLight() {
	
	glDisable(GL_DEPTH_TEST);
	
	glPushMatrix();
	glMultMatrixf(billboadMatrix);
	
	
	// little touch outline...
	ofEnableAlphaBlending();
	float rad_extend = radius + 100;
	//draw circular hailo
	glBegin(GL_TRIANGLE_STRIP);
	int steps = 30;
	float inc = TWO_PI/steps;
	for(int i=0;i<steps+1;i++){
		float x = cos(i*inc);
		float y = sin(i*inc);
		
		if(bDoubleTouched) {
			glColor4f(1, 0, 0, touchAlpha/255.0);
		} else {
			glColor4f(1,1,1, touchAlpha/255.0);
		}
		glVertex3f(x*radius,y*radius, 0);
		glColor4f(1,1,1,0);
		glVertex3f(x*rad_extend,y*rad_extend, 0);
		
	}
	glEnd();
	glPopMatrix();
}

//--------------------------------------------------------------
void ContentBubble::draw() {
	
	// for debuggin...
	//ofSetColor(255, 255, 0);
	//drawSphere(target, 3, 10);
	glEnable(GL_DEPTH_TEST);
	// the main bubble sphere
	ofSetColor(color, alpha);
	ofFill();
	drawSphere(ofVec3f(0, 0, 0), radius, 60);
}

