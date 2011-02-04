/*
 *  ofxPlanarKinect.h
 *
 *  Created by Marek Bereza on 31/01/2011.
 * 
 *  With this class you can track on a 2d plane putting the kinect parallel to the plane.
 *  Effectively it turns a surface into a multitouch input device.
 *
 */

// downsample factor of depth graph
#define ofxPlanarKinect_depthGraphResolution 2

class ofxPlanarKinect: public ofRectangle, public ofBaseDraws {

public:
	ofxPlanarKinect() {
		pixels = NULL;
		depthGraph = NULL;
		mouseIsDown = false;
		width = 640;
		height = 480;
	}
	
	~ofxPlanarKinect() {
		if(pixels!=NULL) {
			delete [] pixels;
		}
		if(depthGraph!=NULL) {
			delete [] depthGraph;
		}
		
	}
	
	void setup() {
		dims = ofVec2f(640, 480); // or whatever the kinect is.
		sliceY = dims.y/2;
		camImg.allocate(dims.x, dims.y, GL_LUMINANCE);
		sliceImg.allocate(dims.x, 1, GL_LUMINANCE);
		numPixels = dims.x*dims.y;
		pixels = new unsigned char[numPixels];
		numDepthGraphPoints = dims.x/ofxPlanarKinect_depthGraphResolution;
		depthGraph = new ofVec2f[numDepthGraphPoints];
		
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
		
		preprocessSlice();
		
		sliceImg.loadData(slice, dims.x, 1, GL_LUMINANCE);
		
		// calculate the depth graph points
		int pos = 0;
		float scale = 1*width/dims.x;
		for(int i = 0; i < dims.x; i+=ofxPlanarKinect_depthGraphResolution) {
			depthGraph[pos] = ofVec2f(x + i*scale, y+slice[i]);
			pos++;
		}
	}
	
	
	void draw() {
		draw(x, y, width, height);
	}
	
	void draw(float x,float y) {
		draw(x, y, getWidth(), getHeight());
	}
	
	
	
	
	
	
	float getHeight() {
		return height;
	}
	float getWidth() {
		return width;
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
			glBegin(GL_LINE_STRIP);
			for(int i = 0 ; i < numDepthGraphPoints; i++)
				glVertex2f(depthGraph[i].x, depthGraph[i].y);
			glEnd();
		}
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
	
	
	void preprocessSlice() {
		// start with at least the first pixel being a lowpass filtered value of all the pixels that are not 0
		if(slice[0]==0) {
			float val = 0;
			for(int i = 1; i < dims.x; i++) {
				if(slice[i]!=0) {
					if(val==0) {
						val = slice[i];
					} else {
						val = val*0.92 + ((float)slice[i])*0.08;
					}
				}
			}
			slice[0] = val;
			if(val>255) printf("First slice pixel too big! %f\n", val);
		}
		
		// flood fill any black out with previous pixels
		for(int i = 1; i < dims.x; i++) {
			if(slice[i]==0) slice[i] = slice[i-1];
		}
		
		
	}
	/** current frame (this gets dynamically allocated and copied to every frame */
	unsigned char *pixels;
	
	/** the start point in 'pixels' of the row of interest */
	unsigned char *slice;
	
	/** This is which row of pixels we slice from */
	int sliceY;

	/** This is the graph that gets drawn to the screen */
	ofVec2f *depthGraph;
	int numDepthGraphPoints;	
	
	int numPixels;
	ofTexture camImg;
	ofTexture sliceImg;
	ofVec2f dims;
	bool mouseIsDown;
};