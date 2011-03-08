#include "testApp.h"

using namespace MSA;


//--------------------------------------------------------------
void testApp::setup() {	 

	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true);
	//fluidSolver.setFadeSpeed(0.000001);
	fluidSolver.setFadeSpeed(0.002f);
	//fluidSolver.setFadeSpeed(1.0);
	fluidSolver.setDeltaT(1);
	fluidSolver.setColorDiffusion(0);
	fluidSolver.setVisc(0.00015);
	fluidDrawer.setup( &fluidSolver );
	particleSystem.setFluidSolver( &fluidSolver );
	particleSystem.setWindowSize(ofVec2f(ofGetWidth(), ofGetHeight()));
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	drawParticles		= true;
	
	ofSetFrameRate(60);
	ofBackground(1, 1, 1);//ofBackground(0, 0, 0);
	ofSetVerticalSync(false);
	
	windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
	pMouse = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
	resizeFluid			= true;
	
	currentEmitter = 0;
	float inc = ofGetWidth()/3.f;
	for(int i = 0; i<NUM_EMITTERS/2; i++)
	{
		forceEmitters[i].set(i*inc, ofGetHeight());
	}

	for(int i = NUM_EMITTERS/2; i<NUM_EMITTERS; i++)
	{
		forceEmitters[i].set((i-NUM_EMITTERS/2)*inc, ofGetHeight()/2);
	}
	
	
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);
}


void testApp::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}


void testApp::update(){
	/*if(resizeFluid)
	{
		
		//float hwRatio = ofGetHeight()/ofGetWidth();
		float hwRatio = 1200/800;
		fluidSolver.setSize(fluidCellsX, fluidCellsX / hwRatio);
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}*/
	
	fluidSolver.update();
	
	for(int i = 0 ; i<NUM_EMITTERS; i++)
	{
	
		ofVec2f vel(0,ofRandom(1.f) * -1);
	
		ofVec2f constrainPos(
							 ofMap(forceEmitters[i].x, 0, 1024, 0.f, 1.f, true),
							 ofMap(forceEmitters[i].y, 0, 768, 0.f, 1.f, true) 
							 );
		
		const float colorMult = 100;
		const float velocityMult = 0.3;
		
		//addToFluid(forceEmitters[i], vel, true, true);
		int index = fluidSolver.getIndexForPos(constrainPos);
		fluidSolver.addForceAtIndex(index, vel * velocityMult);
	}
	
	if(ofGetFrameNum() % 2 == 0 ) {
		
		ofVec2f pos(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		ofVec2f vel(sin(ofGetFrameNum()), cos(ofGetFrameNum()));
		
		addToFluid(pos, vel, true, true);
		
	}
		
}

void testApp::draw()
{
	glColor3f(0.3, 0.09, 0.07);
	//fluidDrawer.draw(0, 0, ofGetWidth(), ofGetHeight());
	particleSystem.updateAndDraw( drawFluid );
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f( eventPos ) / ofGetWidth();
	ofVec2f mouseVel = ofVec2f( eventPos - pMouse ) / ofVec2f(ofGetWidth(), ofGetHeight());
	addToFluid( mouseNorm * ofVec2f( ofGetWindowSize()), mouseVel, true, true );
	pMouse = eventPos;
}

void testApp::mousePressed(int x, int y, int button)
{
	/*if(addForces)
	//{
		ofVec2f eventPos = ofVec2f(x, y);
		ofVec2f mouseNorm = ofVec2f( eventPos ) / ofVec2f(ofGetWidth(), ofGetHeight());
		ofVec2f mouseVel = ofVec2f( eventPos - pMouse ) / ofVec2f(ofGetWidth(), ofGetHeight());
		//addToFluid( mouseNorm, mouseVel, false, true );
		//addToFluid( eventPos, mouseVel, true, true );
		mouseVel *= 100.f;
		//ofVec2f vel(0, -10.f);

		fluidSolver.addForceAtIndex(fluidSolver.getIndexForPos(eventPos), mouseVel);
		pMouse = eventPos;
	//}*/
}	

void testApp::mouseDragged(int x, int y, int button)
{

	/*ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm(eventPos.x/ofGetWidth(), eventPos.y/ofGetHeight());
	ofVec2f mouseVel = ofVec2f( (eventPos.x - pMouse.x) / ofGetWidth(), (eventPos.y - pMouse.y) / ofGetHeight());
	//addToFluid( mouseNorm, mouseVel, false, true );
	addToFluid( mouseNorm, mouseVel, true, true );
	pMouse = eventPos;*/

}

void testApp::keyPressed  (int key)
{
	
	if(key == '1') {
		drawFluid = !drawFluid;
	} else if( key == '2') {
		drawParticles = !drawParticles;
	} else if( key == '3') {
		
	} else if( key == '4') {
		
	} else if( key == '5') {
		
	} else if( key == '6') {
		
	}

	
}

// add force and dye to fluid, and create particles
void testApp::addToFluid( ofVec2f pos, ofVec2f vel, bool addColor, bool addForce ) {
    //float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();    
	//balance the x and y components of speed with the screen aspect ratio
	float aspectRatio = ofGetWidth()/ofGetHeight();
	float speed = vel.x * vel.x  + vel.y * vel.y * aspectRatio * aspectRatio;
    if(speed > 0) {
		ofVec2f constrainPos(
				ofMap(pos.x, 0, 1024, 0.f, 1.f, true),
				ofMap(pos.y, 0, 768, 0.f, 1.f, true) 
				);
		
        const float colorMult = 100;
        const float velocityMult = 30;
		
		int index = fluidSolver.getIndexForPos(constrainPos);
		
		if(addColor) {
			ofColor drawColor;
			drawColor.setHex(0x5E2612, 1.0);
			//drawColor.setHsb((ofGetFrameNum() % 360 ) / 360.0f, 1, 1);
			
			fluidSolver.addColorAtIndex(index, drawColor);// * colorMult);
		}
		
		
		//particleSystem.addParticles( pos * ofVec2f( ofGetWindowSize() ), 5 );
		particleSystem.addParticles( pos, 3 );
		//fluidSolver.addForceAtIndex(index, vel * velocityMult);
    }
}

