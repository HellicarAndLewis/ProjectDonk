
#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	ofAddListener(ofEvents.touchDown,this,&testApp::touchDown);
	ofAddListener(ofEvents.touchUp,this,&testApp::touchUp);
	ofAddListener(ofEvents.touchMoved,this,&testApp::touchMoved);
	ofAddListener(ofEvents.touchDoubleTap,this,&testApp::touchDoubleTap);
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	// initialize all of the Thing particles
	for(int i=0; i<NUM_POINTS; i++) balls[i].init();
}


//--------------------------------------------------------------
void testApp::update()
{
	for(int i=0; i<NUM_POINTS; i++) balls[i].update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	for(int i=0; i<NUM_POINTS; i++) balls[i].draw();
}

void testApp::exit() {
	printf("exit()\n");
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){
	printf("touch %i down at (%i,%i)\n", touch.id, touch.x,touch.y);
	balls[touch.id].moveTo(touch.x, touch.y);
}
//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){
	printf("touch %i moved at (%i,%i)\n", touch.id, touch.x,touch.y);
	balls[touch.id].moveTo(touch.x, touch.y);
}
//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){
	printf("touch %i up at (%i,%i)\n", touch.id, touch.x,touch.y);
}
//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){
	printf("touch %i double tap at (%i,%i)\n", touch.id, touch.x,touch.y);
}
//--------------------------------------------------------------
void testApp::lostFocus()
{
	cout<<"lostFocus"<<endl;
}
//--------------------------------------------------------------
void testApp::gotFocus()
{
	cout<<"gotFocus"<<endl;
}
//--------------------------------------------------------------
void testApp::gotMemoryWarning()
{
	cout<<"MemoryWarning"<<endl;
}

