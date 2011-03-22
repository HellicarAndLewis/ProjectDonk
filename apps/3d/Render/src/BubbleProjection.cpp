/**
 * BubbleProjection.cpp
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */

#include "BubbleProjection.h"
#include "testApp.h"
#include "Mode.h"
#include "AudioData.h"
#include "App.h"


#define DEBUG_INTERATIONS 1
int		debugCount = 0;

//--------------------------------------------------------
static bool shouldRemoveTouch(BubbleTouch &t) {
	return t.bRemove;	
}

static bool shouldRemoveConstraint(TouchedConstraint * tc) {
	bool bRemove = false;
	if(tc != NULL) {
		bRemove = tc->bRemove;
		if(bRemove) {
			tc->destroy();
			delete tc;
			tc = NULL;
		}
	}
	return bRemove;
}

//--------------------------------------------------------
BubbleProjection::BubbleProjection() {
	interactiveArea     = ofRectangle(100,100,900,500);
	activeInteraction   = NULL;
	previousInteraction = NULL;
	nextInteraction     = NULL;
	touchPadding		= 10.0;
	//drawingParticles	= false;
	drawingParticles	= true;
	drawingChampagne	= true;
}

//--------------------------------------------------------
/** This gets called straight after allocate() */
void BubbleProjection::setup() {	
	
	bullet.init();
	bullet.setGravity(0, 0, 0);
	bullet.camera = &camera;
	
	// setup all the interaction modes
	interactions.push_back(new InteractionBuzz());
	interactions.push_back(new InteractionInspiration());
	interactions.push_back(new InteractionInterview());
	interactions.push_back(new InteractionVote());
	interactions.push_back(new InteractionPerformance());
	
	for (int i=0; i<interactions.size(); i++) {
		interactions[i]->bullet = &bullet;
		interactions[i]->interactiveRect = ofRectangle(100, 100, getWidth(), getHeight());
		interactions[i]->setup();
	}
	
	// just for testing...
	// we will get an event that tells us the mode
	
	activeInteraction   = interactions[MODE_BUZZ];//INSPIRATION];
	activeInteraction->animatedIn();
	activeInteraction->bActive = true;
	
	// we have a ref to the previous interaction
	// so that we can have one animated out as the 
	// next one animates in.
	previousInteraction = NULL;
	
	if(drawingChampagne)
		champagne.setup();
	
	// probably only want to do this when the particle system is used //
	if(drawingParticles)
		particleSys.init(ofVec2f(2000, 2000));
	
	bTouchDown = false;
}

//--------------------------------------------------------
BaseInteraction * BubbleProjection::getInteraction(int mode) {
	if(mode		 == MODE_BUZZ)			return interactions[mode];
	else if(mode == MODE_INSPIRATION)	return interactions[mode];
	else if(mode == MODE_INTERVIEW)		return interactions[mode];
	else if(mode == MODE_VOTE)			return interactions[mode];
	else if(mode == MODE_PERFORMANCE)	return interactions[mode];
	else return NULL;	
}

//--------------------------------------------------------
void BubbleProjection::interactionModeChange(string modeName) {
	
	printf("The New Interaction Mode: %s\n", modeName.c_str());	
	
	if(activeInteraction) {
		activeInteraction->animatedOut();
		previousInteraction = activeInteraction;
		previousInteraction->bActive = false;
	}
	
	// still need to add the rest...
	int mode = -1;
	if(modeName		 == "buzz")	{
		mode = MODE_BUZZ;
	}
	else if(modeName == "inspiration") {
		mode = MODE_INSPIRATION;
		particleSys.particleColor.set(1, 1, 1, 0.75);
	}
	else if(modeName == "interview") {
		
		mode = MODE_INTERVIEW;
	}
	else if(modeName == "vote") {
		mode = MODE_VOTE;
		particleSys.particleColor.set(0, 1, 1, 0.75);
	}
	else if(modeName == "performance") {
		mode = MODE_PERFORMANCE;
		particleSys.particleColor.set(1, 1, 0, 0.75);
	}
	
	particleSys.setInteractionMode(mode);
	
	
	if(mode != -1) {
		nextInteraction = interactions[mode];
		nextInteraction->bActive = true;
	}
}

//--------------------------------------------------------
void BubbleProjection::update() {
	
	
	//if(bAnimateOut) {
	//		float time = (ofGetElapsedTimeMillis()-animatedOutTimer) / 1000.0;
	//		if(time > MAX_ANIMATION_TIME || bAllOffScreen && !bDoneAnimatingOut) {
	//			bDoneAnimatingOut = true;
	//			killallBubbles();
	//		}
	//	}
	//	
	
	for (int i=0; i<interactions.size(); i++) {
		if(!interactions[i]->bActive && interactions[i]->bAnimateOut) {
			float time = (ofGetElapsedTimeMillis()-interactions[i]->animatedOutTimer) / 1000.0;
			if(time > 6.0 && !interactions[i]->bDoneAnimatingOut) {
				interactions[i]->bDoneAnimatingOut = true;
				interactions[i]->killallBubbles();
				printf("*** emergency clean up ***\n");
			}
		}
	}
	
	// -------------------
	// Previous Interaction
	// -------------------
	if(previousInteraction) {
		
		previousInteraction->nTouches = touches.size();
		previousInteraction->update();
		
		if(previousInteraction->bDoneAnimatingOut && previousInteraction->bAnimateOut) {
			previousInteraction->putToRest();
			printf(" *** done animating out: %s ***\n", previousInteraction->name.c_str());
			previousInteraction = NULL;
			
			if(nextInteraction) {
				activeInteraction = nextInteraction;
				if(activeInteraction) activeInteraction->animatedIn();
				nextInteraction = NULL;
			}
		}
		
		
	}
	
	// -------------------
	// Active Interaction
	// -------------------
	if(activeInteraction) {
		activeInteraction->nTouches = touches.size();
		activeInteraction->update();
	}
	
	// -------------------
	// Update the touches
	// -------------------
	bool bNeedToCleanUp = false;
	for (int i=0; i<touches.size(); i++) {
		touches[i].update();
		if(touches[i].bRemove) bNeedToCleanUp = true;
	}
	
	if(bNeedToCleanUp) touches.erase(touches.begin(), partition(touches.begin(), touches.end(), shouldRemoveTouch));
	
	
	// -------------------
	// Bullet etc...
	// -------------------
	bullet.update();
	bubbleShader.update();
	
	// -------------------
	// pretty stuff
	// -------------------
	if(drawingChampagne) {
		champagne.update();
	}
	
	if(drawingParticles) {
		particleSys.update();
	}
	
	
	//----------------
	// check to clean up touch constraints
	// need to check that this works!
	if(!bTouchDown && activeInteraction) {
		
		for (int i=0; i<touchConstraints.size(); i++) {
			touchConstraints[i]->bRemove  = true;
		}
		
		touchConstraints.erase(touchConstraints.begin(),
							   partition(touchConstraints.begin(), 
										 touchConstraints.end(), 
										 shouldRemoveConstraint));
		
		/*for (int i=touchConstraints.size()-1; i>=0; i--) {
			if(touchConstraints[i] != NULL) {
				touchConstraints[i]->destroy();
				delete touchConstraints[i];
				touchConstraints[i] = NULL;
			}
			touchConstraints.erase(touchConstraints.begin() + i);
		}*/
		
	}
	
	
	
}



void BubbleProjection::drawOnModel(Model *model) {
	
	//glViewport(0, 0, ofGetWidth()*2, ofGetHeight()*2);
	// scale the texture matrix so we can use normalized tex coords
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glScalef(width, -height, 1);
	glTranslatef(0, -1, 0);
	glMatrixMode(GL_MODELVIEW);
	
	
	// draw the model (assuming it also draws normalized texCoords
	bind();
	glColor3f(1, 1, 1);
	model->drawModel();
	unbind();
	
	// reset the texture matrix
	glMatrixMode(GL_TEXTURE);
	//	glLoadIdentity();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	//particleSys.draw(true); // just checking if these can draw outside of FBO
}

//--------------------------------------------------------
void BubbleProjection::draw() {
	
	ofEnableAlphaBlending();
	
	// clear the FBO
	// audioReactiveness: 0 is not audio reactive, 1 is fully audio reactive
	float audioReactiveness = Donk::Mode::getInstance()->getValue("Background Audio-reactiveness");
	float volume = Donk::AudioData::getInstance()->getVolume(0);
	float amp = (1.f - audioReactiveness) + audioReactiveness*volume;//1 - volume *(1-audioReactiveness);
	
	// we're doing background colour in testApp on the actual sculpture itself
	/*ofClear(amp*Donk::Mode::getInstance()->getValue("Top BG Red"), 
	 amp*Donk::Mode::getInstance()->getValue("Top BG Green"), 
	 amp*Donk::Mode::getInstance()->getValue("Top BG Blue"), 255);*/
	
	// empty the texture
	ofClear(0, 0, 0, 0);
	
	// draw center of the app
	if(((Donk::App*)ofGetAppPtr())->guiEnabled){
		glLineWidth(3);
		ofNoFill();
		ofSetColor(255, 0, 0,128);
		glBegin(GL_LINES);
		glVertex2f(getWidth()/2-10, getHeight()/2);
		glVertex2f(getWidth()/2+10, getHeight()/2);
		glVertex2f(getWidth()/2, getHeight()/2-10);
		glVertex2f(getWidth()/2, getHeight()/2+10);
		glEnd();
		glLineWidth(1);
	}
	
	if(drawingParticles) {
		//particleSys.draw(false);
	}
	
	if(drawingChampagne){
		champagne.draw();	
	}
	
	// -------------------
	if(previousInteraction) {
		previousInteraction->drawContent();
		previousInteraction->drawSphere(&bubbleShader);
	}
	
	// -------------------
	if(activeInteraction)   {
		activeInteraction->drawContent();
		activeInteraction->drawSphere(&bubbleShader);
	}
	
	
	
	// -------------------
	// draw touches
	// -------------------
	for (int i=0; i<touches.size(); i++) {
		if(!touches[i].bChildOfGesture) {
			ofVec2f tp = mapToInteractiveArea(touches[i].getPosition());
			
			touches[i].drawTouch(tp);
			
			if(ofGetFrameNum()%12==0)champagne.particles.push_back( new BrownianObject( tp, 10));
			
			particleSys.addForceAtPoint( tp );
		}
		//particleSys.addForceAndParticle(tp, false, true);
	}
	
}



//--------------------------------------------------------
/** A bubble was just received */
void BubbleProjection::bubbleReceived(Donk::BubbleData *bubbleData) {
	
	// printf("%s %s\n", bubbleData->text.c_str(), bubbleData->userName.c_str());
	
	/*
	 ofVec3f center(getWidth()/2, 0, 0);
	 ofVec3f startPos(center.x + ofRandom(-300, 300), getHeight(), ofRandom(-100, 100));
	 float   radius = 80;
	 
	 ContentBubble * bubble = new ContentBubble();
	 
	 bubble->data	  = bubbleData;
	 bubble->radius    = radius;
	 bubble->rigidBody = bullet.createSphere(startPos, radius, 1);
	 bubble->createContentBubble();
	 bubble->target.set(center.x + ofRandom(-300, 300), ofRandom(500, getHeight()-300), 0);
	 bubbles.push_back(bubble);
	 */
	
	if(activeInteraction) {
		activeInteraction->newBubbleRecieved(bubbleData);	
	}
}

//--------------------------------------------------------
ofRectangle &BubbleProjection::getInteractiveArea() {
	return interactiveArea;
}

//--------------------------------------------------------
void BubbleProjection::addTouchConstraints(ContentBubble * bubble) {
	
	if(bubble->bTouched) return;
	TouchedConstraint * touchCon = new TouchedConstraint();
	touchCon->setTouchBody(bullet.world, bubble->rigidBody->body);
	touchConstraints.push_back(touchCon);
	
	printf("Constraint Made: %p\n", bubble);
	
}

//--------------------------------------------------------
void BubbleProjection::removeTouchConstraint(ContentBubble * bubble) {
	
	if(bubble != NULL) {
		bubble->touchID = -1;
		TouchedConstraint * touchCon = NULL;
		int					removeInd = -1;
		for (int i=0; i<touchConstraints.size(); i++) {
			if(touchConstraints[i]->body == bubble->rigidBody->body) {
				touchCon = touchConstraints[i];
				removeInd = i;
				printf("remove this cont\n");
			}
		}
		
		if(removeInd != -1) {
			touchCon->bRemove = true;
		}
	}
	
	for (int i=0; i<touchConstraints.size(); i++) {
		if(touchConstraints[i]->body == NULL) {
			touchConstraints[i]->bRemove  = true;
		}
	}
	
	touchConstraints.erase(touchConstraints.begin(),
						   partition(touchConstraints.begin(), touchConstraints.end(), shouldRemoveConstraint));

	printf("Touch Constraint Size:%i\n", (int)touchConstraints.size());
}

//--------------------------------------------------------
void BubbleProjection::touchDown(float x, float y, int touchId) {
	
	// cout << touchId << endl;
	bTouchDown = true;
	
	ofVec2f touchCoords(x, y);
	ofVec2f pos = mapToInteractiveArea(touchCoords);
	if(activeInteraction) {
		
		
		
		InteractionVote * voteInteraction = NULL;
		bool bInVoteMode = false;
		if(activeInteraction->name == "vote") {
			voteInteraction = (InteractionVote*)activeInteraction;	
			if(voteInteraction) {
				bInVoteMode = true;
			}
		}
		
		// ----------
		// vote mode
		// -----------
		if(bInVoteMode) {
			if(voteInteraction->bMadeVoteBubbles) {
				for(int i=0; i<2; i++) {
					
					ContentBubble * bubble = (ContentBubble*)voteInteraction->voteBubbles[i];
					if(bubble) {
						if(!bubble->bTouched) {
							ofVec2f p1  = pos;
							ofVec2f p2  = bubble->rigidBody->getPosition();
							float	dis = p1.distance(p2);
							if(dis < bubble->radius + touchPadding) {
								addTouchConstraints(bubble);
								bubble->bTouched = true;
								break;
							}
							else {
								bubble->bTouched = false;			
							}
						}
					}
				}
			}
			
		}
		
		// -----------
		// normal mode
		// ------------
		else {
			
			
			
			// first we check to see if any bubbles are using 
			// the touchID. if not then we can make a constraint
			bool bTouchedIDUsed = false;
			for(int i=0; i<activeInteraction->bubbles.size(); i++) {
				ContentBubble * bubble = activeInteraction->bubbles[i];
				// shouldn't have to test for null here!!
				if (bubble!=NULL && bubble->touchID == touchId) {
					bTouchedIDUsed = true;
				}
			}
			
			if(!bTouchedIDUsed) {
				for(int i=0; i<activeInteraction->bubbles.size(); i++) {
					
					ContentBubble * bubble = activeInteraction->bubbles[i];
					// shouldn't have to test for null here!!
					if(bubble==NULL || (activeInteraction->name == "buzz" && !bubble->bAlive)) continue;
					
					ofVec2f p1  = pos;
					ofVec2f p2  = bubble->rigidBody->getPosition();
					float	dis = p1.distance(p2);
					if(dis < bubble->radius + touchPadding) {
						bubble->touchID = touchId;
						addTouchConstraints(bubble);
						bubble->bTouched = true;
						break;
					}
					else {
						bubble->bTouched = false;			
					}
				}
			}
		}
		
	}
	
	
	// find the closest point to the new touch
	float minSqrDist = FLT_MAX; // do squares
	int minTouchId   = -1;
	
	
	for (int i=0; i<touches.size(); i++) {
		float sqrDist = touchCoords.squareDistance(touches[i].getPosition());
		
		if(sqrDist < minSqrDist) {
			//printf("%f\n", sqrDist);
			minTouchId = touches[i].id;
			minSqrDist = sqrDist;
		}
		
	}
	
	// add the touch
	touches.push_back(BubbleTouch());
	touches.back().setPosition(touchCoords);
	touches.back().id = touchId;
	
	// the minimum distance between the 2 closest touches 
	// in order for it to be a double touch.
	float doubleTouchDist = 0.1;
		
	// if there's another touch, and it's close enough, call doubleTouchGesture
	if(minTouchId!=-1 && sqrt(minSqrDist) < doubleTouchDist) { 
		doubleTouchGesture(touchId, minTouchId);
	}
	
}

//--------------------------------------------------------
void BubbleProjection::touchMoved(float x, float y, int touchId) {
	
	// cout << touchId << endl;
	
	
	ofVec2f p(x, y);
	bool bTouchFromDoubleTouch = false;
	for (int i=0; i<touches.size(); i++) {
		if(touches[i].id == touchId) {
			touches[i].setPosition( p );
			if (touches[i].bChildOfGesture) {
				bTouchFromDoubleTouch = true;
			}
		}
	}
	
	ofVec2f pos = mapToInteractiveArea( p );
	
	
	if(activeInteraction) {
		
		InteractionVote * voteInteraction = NULL;
		bool bInVoteMode = false;
		if(activeInteraction->name == "vote") {
			voteInteraction = (InteractionVote*)activeInteraction;	
			if(voteInteraction) {
				bInVoteMode = true;
			}
		}
		
		if(bInVoteMode) {
			
			if(voteInteraction->bMadeVoteBubbles) {
				for(int i=0; i<2; i++) {
					
					ContentBubble * bubble = (ContentBubble*)voteInteraction->voteBubbles[i];				
					if(bubble) {
						if(bubble->bTouched == false) {
							ofVec2f p1  = pos;
							ofVec2f p2  = bubble->rigidBody->getPosition();
							float	dis = p1.distance(p2);
							if(dis < bubble->radius + touchPadding) {
								addTouchConstraints(bubble);
								bubble->bTouched = true;
							}
						}
						// update the interaction touch constraints
						for (int j=0; j<touchConstraints.size(); j++) {
							TouchedConstraint * tc = touchConstraints[j];
							if(tc->body == bubble->rigidBody->body) {
								touchConstraints[j]->updateTouchConstraint(pos);
							}
						}	
					}
					
					
				}
				
				
			}
			
		}
		
		
		
		else {
			
			// first we check to see if any bubbles are using 
			// this touchID. if not then we can make a constraint
			bool bTouchedIDUsed = false;
			
			for(int i=0; i<activeInteraction->bubbles.size(); i++) {
				ContentBubble * bubble = activeInteraction->bubbles[i];
				
				if(bubble==NULL || (activeInteraction->name == "buzz" && !bubble->bAlive)) continue;
				
				if (bubble->touchID == touchId) {
					bTouchedIDUsed = true;
				}
				
				printf( "%i - id:%i\n", i, bubble->touchID);
			}
			
			if(bTouchFromDoubleTouch) bTouchedIDUsed = true;

			for(int i=0; i<activeInteraction->bubbles.size(); i++) {
				
				ContentBubble * bubble = activeInteraction->bubbles[i];
				
				if(bubble==NULL || (activeInteraction->name == "buzz" && !bubble->bAlive)) continue;
				
				// update the interaction touch constraints
				for (int j=0; j<touchConstraints.size(); j++) {
					TouchedConstraint * tc = touchConstraints[j];
					if(tc->body == bubble->rigidBody->body) {
						touchConstraints[j]->updateTouchConstraint(pos);
					}
				}	
				
				if(!bTouchedIDUsed) {
					ofVec2f p1  = pos;
					ofVec2f p2  = bubble->rigidBody->getPosition();
					float	dis = p1.distance(p2);
					
					if(dis < bubble->radius+touchPadding && !bubble->bTouched) {
						bubble->touchID = touchId;
						addTouchConstraints(bubble);
						bubble->bTouched = true;
						break;
					}
				}
				
			}
			
		}
		
		
	}
	
	
	
}

//--------------------------------------------------------
void BubbleProjection::touchUp(float x, float y, int touchId) {
	
	for (int i=0; i<touches.size(); i++) {
		if(touches[i].id == touchId) {
			//printf("*** %p\n", touches[i].secondTouch);
			if(touches[i].secondTouch) {
				
				touches[i].secondTouch->bChildOfGesture = false;
				touches[i].secondTouch->radiusDes = 30;
				touches[i].secondTouch->touchAlphaDes = 200;
			}
		}
	}
	
	bTouchDown = false;
	
	ofVec2f pos = mapToInteractiveArea(ofVec2f(x, y));
	
	if(activeInteraction) {
		
		InteractionVote * voteInteraction = NULL;
		bool bInVoteMode = false;
		if(activeInteraction->name == "vote") {
			voteInteraction = (InteractionVote*)activeInteraction;	
			if(voteInteraction) {
				bInVoteMode = true;
			}
		}
		
		
		if(bInVoteMode) {
			for(int i=0; i<2; i++) {
				
				if(voteInteraction->bMadeVoteBubbles) {
					ContentBubble * bubble = (ContentBubble*)voteInteraction->voteBubbles[i];				
					if(bubble != NULL) {
						if(bubble->bTouched) {
							
							bubble->bTouched = false;
							removeTouchConstraint(bubble);
							
						}
					}
				}
			}
			
		}
		
		
		else {
			
			for(int i=0; i<activeInteraction->bubbles.size(); i++) {
				
				ContentBubble * bubble = activeInteraction->bubbles[i];
				
				if(bubble != NULL) {
					
					if(bubble->bTouched) {
						
						bubble->releaseTouch();
						
						if (activeInteraction->name == "interview") {
							bubble->setTarget(pos.x, pos.y);
							bubble->setLoopStart( getHeight() );
						}
						
						if (activeInteraction->name == "inspiration") {
							bubble->setTarget(pos.x, pos.y);
						}
						
						
						if (activeInteraction->name == "buzz") {
							if( !activeInteraction->bAnimateOut ) bubble->setTarget(pos.x, pos.y);
						}
						
						if (activeInteraction->name == "performance") {
							bubble->setTarget(pos.x, pos.y);
							bubble->performanceStartTarget = bubble->target;
						}
						
						bubble->bTouched = false;
						removeTouchConstraint(bubble);
					}
				}
			}
			
		}
	}
	
	
	// remove old touches...
	for (int i=0; i<touches.size(); i++) {
		if(touches[i].id == touchId) {
			touches[i].bRemove = true;
		}
	}
	
	
}

//--------------------------------------------------------
ofVec2f BubbleProjection::mapToInteractiveArea(ofVec2f inPoint) {
	return ofVec2f(interactiveArea.x + interactiveArea.width  * inPoint.x,
				   interactiveArea.y + interactiveArea.height * inPoint.y);
}

//--------------------------------------------------------
void BubbleProjection::doubleTouchGesture(int touch1Id, int touch2Id) {
	
	BubbleTouch *t1 = NULL;
	BubbleTouch *t2 = NULL;
	
	printf("%i %i\n", touch1Id, touch2Id);
	for (int i=0; i<touches.size(); i++) {
		if(touches[i].id == touch1Id) t1 = &touches[i];
		if(touches[i].id == touch2Id) t2 = &touches[i];
	}
	
	if(t1 && t2) {
		ofVec2f doubleTouchCenter = (t1->getPosition() + t2->getPosition())/2;
		
		// fade out this touch we are not using it 
		// anymore cause we have a double gesture
		t2->touchAlphaDes = 0;
		t2->radiusDes = 0;
		t2->bChildOfGesture = true; // kinda a hack...
		
		t1->secondTouch = t2;
		t1->setPosition(doubleTouchCenter); 
		t1->enableGesture();
		
		
		
		// map for the active interaction
		doubleTouchCenter = mapToInteractiveArea(doubleTouchCenter);
		
		// send the active interaction the double gesture!
		if(activeInteraction) {
			activeInteraction->doubleTouched(doubleTouchCenter);	
		}
		
		printf("doubleTouchGesture\n");
	}
}
