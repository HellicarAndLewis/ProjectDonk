#include "testApp.h"

using namespace MSA;


//--------------------------------------------------------------
void testApp::setup() {	 

	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true);
	fluidSolver.setFadeSpeed(0.00001);
	fluidSolver.setDeltaT(0.15).setColorDiffusion(0);
	fluidSolver.setVisc(0.00015);
	fluidDrawer.setup( &fluidSolver );
	particleSystem.setFluidSolver( &fluidSolver );
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	drawParticles		= true;
	
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	ofSetVerticalSync(false);
	
	windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
	pMouse = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
	resizeFluid			= true;
	
	addForces = false;
	
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);
}


void testApp::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}


// add force and dye to fluid, and create particles
void testApp::addToFluid( ofVec2f pos, ofVec2f vel, bool addColor, bool addForce ) {
    //float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
	float aspectRatio = ofGetWidth()/ofGetHeight();
	float speed = vel.x * vel.x  + vel.y * vel.y * aspectRatio * aspectRatio;
    if(speed > 0) {
		pos.x = constrain(pos.x, 0.0f, 1.0f);
		pos.y = constrain(pos.y, 0.0f, 1.0f);
		
        const float colorMult = 100;
        const float velocityMult = 30;
		
        int index = fluidSolver.getIndexForPos(pos);
		
		if(addColor) {
			ofColor drawColor;
			drawColor.setHsb((ofGetFrameNum() % 360 ) / 360.0f, 1, 1);
			//			Color drawColor;
			//			drawColor.setHSV(( getElapsedFrames() % 360 ) / 360.0f, 1, 1 );
			
			fluidSolver.addColorAtIndex(index, drawColor * colorMult);
			
			if( drawParticles )
				particleSystem.addParticles( pos * ofVec2f( ofGetWindowSize() ), 10 );
		}
		
		if(addForce)
			fluidSolver.addForceAtIndex(index, vel * velocityMult);
		
    }
}


void testApp::update(){
	if(resizeFluid) 	{
		
		//float hwRatio = ofGetHeight()/ofGetWidth();
		float hwRatio = 1200/800;
		fluidSolver.setSize(fluidCellsX, fluidCellsX / hwRatio);
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
	
	
	fluidSolver.update();
}

void testApp::draw(){
//	if( drawFluid ) {
//		glColor3f(1, 1, 1);
//		fluidDrawer.draw(0, 0, ofGetWidth(), ofGetHeight());
	//} else {
	//if( ofGetFrameNum()%5==0) fadeToColor( 0, 0, 0, 0.1f );
	//}
	//if( drawParticles )
		particleSystem.updateAndDraw( drawFluid );
}


void testApp::windowResized(int w, int h) {
	particleSystem.setWindowSize( ofVec2f( w, h ) );
}


void testApp::keyPressed  (int key){ 
    switch(key) {
		case '1':
			fluidDrawer.setDrawMode(MSA::kFluidDrawColor);
			break;
			
		case '2':
			fluidDrawer.setDrawMode(MSA::kFluidDrawMotion);
			break;
			
		case '3':
			fluidDrawer.setDrawMode(MSA::kFluidDrawSpeed);
			break;
			
		case '4':
			fluidDrawer.setDrawMode(MSA::kFluidDrawVectors);
			break;
			
		case '0':
			addForces ^= true;
			break;
			
		case 'd':
			drawFluid ^= true;
			break;
			
		case 'p':
			drawParticles ^= true;
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'r':
			fluidSolver.reset();
			break;
			
		case 'b': {
			//Timer timer;
			const int ITERS = 3000;
			//timer.start();
			for( int i = 0; i < ITERS; ++i ) fluidSolver.update();
			//timer.stop();
			//cout << ITERS << " iterations took " << timer.getSeconds() << " seconds." << std::endl;
		}
			break;
			
    }
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	/*ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f( eventPos ) / ofGetWidth();
	ofVec2f mouseVel = ofVec2f( eventPos - pMouse ) / ofVec2f(ofGetWidth(), ofGetHeight());
	addToFluid( mouseNorm, mouseVel, true, true );
	pMouse = eventPos;*/
}

void testApp::mousePressed(int x, int y, int button)
{
	if(addForces)
	{
		ofVec2f eventPos = ofVec2f(x, y);
		ofVec2f mouseNorm = ofVec2f( eventPos ) / ofVec2f(ofGetWidth(), ofGetHeight());
		ofVec2f mouseVel = ofVec2f( eventPos - pMouse ) / ofVec2f(ofGetWidth(), ofGetHeight());
		//addToFluid( mouseNorm, mouseVel, false, true );
		//addToFluid( mouseNorm, mouseVel, true, true );
		mouseVel *= 100.f;
		ofVec2f vel(0, -2000.f);
		fluidSolver.addForceAtPos(eventPos, vel);
		pMouse = eventPos;
	}
}	

void testApp::mouseDragged(int x, int y, int button)
{

	ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f( eventPos ) / ofVec2f(ofGetWidth(), ofGetHeight());
	ofVec2f mouseVel = ofVec2f( eventPos - pMouse ) / ofVec2f(ofGetWidth(), ofGetHeight());
	//addToFluid( mouseNorm, mouseVel, false, true );
	addToFluid( mouseNorm, mouseVel, true, true );
	pMouse = eventPos;

}

