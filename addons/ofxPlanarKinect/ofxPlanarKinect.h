/*
 *  ofxPlanarKinect.h
 *
 *  Created by Marek Bereza on 31/01/2011.
 * 
 *  With this class you can track on a 2d plane putting the kinect parallel to the plane.
 *  Effectively it turns a surface into a multitouch input device.
 *
 */


class ofxPlanarKinect: public ofRectangle, public ofBaseDraws {

public:
	ofxPlanarKinect() {
		pixels = NULL;
		mouseIsDown = false;
		width = 640;
		height = 480;
	}
	
	~ofxPlanarKinect() {
		if(pixels!=NULL) {
			delete [] pixels;
		}
		
	}
	
	void setup() {
		dims = ofVec2f(640, 480); // or whatever the kinect is.
		sliceY = dims.y/2;
		camImg.allocate(dims.x, dims.y, GL_LUMINANCE);
		sliceImg.allocate(dims.x, 1, GL_LUMINANCE);
		numPixels = dims.x*dims.y;
		pixels = new unsigned char[numPixels];
		
	}
	
	
	void update(unsigned char *pixels) {
		if(pixels==NULL) {
			ofLog(OF_LOG_ERROR, "ofxPlanarKinect(): setup() not called, or set up incorrectly!");
		}
		
		// copy the whole frame
		memcpy(this->pixels, pixels, numPixels);

		// then make a reference to the slice
		int offset = sliceY*dims.x;
		slice = this->pixels + offset;

		camImg.loadData(this->pixels, dims.x, dims.y, GL_LUMINANCE);
		sliceImg.loadData(slice, dims.x, 1, GL_LUMINANCE);
	}
	
	
	void draw() {
		draw(x, y, width, height);
	}
	
	void draw(float x,float y) {
		draw(x, y, getWidth(), getHeight());
	}
	
	
	void draw(float x,float y,float w, float h) {
		this->x = x;
		this->y = y;
		width = w;
		height = h;
		
		ofSetHexColor(0xFFFFFF);
		if(mouseIsDown) {
			camImg.draw(x,y,width,height);
		
			ofSetHexColor(0xFF0000);
			ofNoFill();
			ofRect(x, y + (sliceY-1)*height/dims.y, width, 3);
			ofFill();
		} else {
			ofSetHexColor(0xFFFFFF);
			sliceImg.draw(x,y,width,height);
			ofSetHexColor(0xFF0000);
			ofDrawBitmapString("Click mouse to choose a slice", x+3,y+14);
		}
	}
	
	
	
	float getHeight() {
		return height;
	}
	float getWidth() {
		return width;
	}
	
	
	void mousePressed(float x, float y, int button) {
		mouseIsDown = inside(x,y);
		if(mouseIsDown) {
			sliceY = (y - this->y)*dims.y/height;
		}
	}
	
	void mouseReleased(float x, float y, int button) {
		mouseIsDown = false;
	}
	
	void mouseDragged(float x, float y, int button) {
		mousePressed(x, y, button);
	}

	
	
private:
	
	/** current frame (this gets dynamically allocated and copied to every frame */
	unsigned char *pixels;
	
	/** the start point in 'pixels' of the row of interest */
	unsigned char *slice;
	
	/** This is which row of pixels we slice from */
	int sliceY;
	int numPixels;
	ofTexture camImg;
	ofTexture sliceImg;
	ofVec2f dims;
	bool mouseIsDown;
};