/**
 *  ofxProjectorBlend
 *  
 * Copyright 2010 (c) James George, http://www.jamesgeorge.org
 * in collaboration with FlightPhase http://www.flightphase.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 *
 * This class is an openFrameworks wrapper for this technique:
 * from http://local.wasp.uwa.edu.au/~pbourke/texture_colour/edgeblend/
 * Original Core Image filter converted by James George ( http://www.jamesgeorge.org/ )
 * for ofxProjectorBlend addon for openFrameworks ( http://www.openframeworks.cc )
 *
 * Originally used in the Infiniti MOI project for projector blending ( http://vimeo.com/14588336 )
 *
 * This shader is capable of blending on all sides, but ofxProjectorBlend currently
 * just uses the shader for two projections at once.
 *
 */

#include "ofxProjectorBlend.h"


ofxProjectorBlend::ofxProjectorBlend()
{
	showBlend = true;
	fullTexture = NULL;
	channelOne = NULL;
	channelTwo = NULL;
	blendShader = NULL;
#ifdef USE_XML_GUI
	gui = NULL;
#endif
}

void ofxProjectorBlend::setup(int resolutionWidth, int resolutionHeight, int _pixelOverlap, ofxProjectorBlendLayout layout)
{
	this->layout = layout;
	
	pixelOverlap = _pixelOverlap;
	
	singleChannelWidth = resolutionWidth;
	singleChannelHeight = resolutionHeight;
	
	
	if(layout==ofxProjectorBlend_Vertical) {
		fullTextureWidth = singleChannelWidth;
		fullTextureHeight = resolutionHeight*2 - pixelOverlap;
	} else if(layout==ofxProjectorBlend_Horizontal) {
		fullTextureWidth = resolutionWidth*2 - pixelOverlap;
		fullTextureHeight = singleChannelHeight;
	} else {
		ofLog(OF_LOG_ERROR, "ofxProjectorBlend: You have used an invalid ofxProjectorBlendLayout in ofxProjectorBlend::setup()");
	}
	
	
	

	
	fullTexture = new ofFbo();
	fullTexture->setup(fullTextureWidth, fullTextureHeight, GL_RGB);

	channelOne = new ofFbo();
	channelTwo = new ofFbo();
	
	channelOne->setup(singleChannelWidth, singleChannelHeight, GL_RGB);
	channelTwo->setup(singleChannelWidth, singleChannelHeight, GL_RGB);
	
	
	
	
	blendShader = new ofShader();
	blendShader->setup("shader/SmoothEdgeBlend.vert","shader/SmoothEdgeBlend.frag", "");
}

void ofxProjectorBlend::begin()
{
	fullTexture->begin();
	ofPushStyle();
}


float ofxProjectorBlend::getCanvasWidth()
{
	return fullTextureWidth;
}

float ofxProjectorBlend::getCanvasHeight()
{
	return fullTextureHeight;
}


#ifdef USE_SIMPLE_GUI
void ofxProjectorBlend::addGuiPage()
{
	gui.addPage("Projector Blend");
	gui.addToggle("Show Blend", showBlend);
	gui.addSlider("Blend Power", blendPower, 0.0, 4.0);
	gui.addSlider("Gamma", gamma, 0.0, 4.0);
	gui.addSlider("Luminance", luminance, 0.0, 4.0);
	
	gui.page("Projector Blend").setXMLName("../../../ProjectorBlendSettings.xml");
}	

#endif

#ifdef USE_XML_GUI
ofxXmlGui *ofxProjectorBlend::getGui() {
	if(gui==NULL) {
		gui = new ofxXmlGui();
		gui->setup(0, 0, 200);
		gui->addToggle("Show Blend", showBlend);
		gui->addSlider("Blend Power", blendPower, 0, 4);
		gui->addSlider("Gamma", gamma, 0, 4);
		gui->addSlider("Luminance", luminance, 0, 4);
		gui->enableAutoSave("");
	}
	
	return gui;
}
#endif
void ofxProjectorBlend::end()
{
	fullTexture->end();
	ofPopStyle();
	
	ofPushStyle();{
		ofEnableAlphaBlending();
		ofSetRectMode(OF_RECTMODE_CORNER);

		channelOne->begin();
		fullTexture->draw(0,0, getCanvasWidth(), getCanvasHeight());
		channelOne->end();
		
		if(layout==ofxProjectorBlend_Horizontal) {
			channelTwo->begin();
			fullTexture->draw(-singleChannelWidth+pixelOverlap, 0, getCanvasWidth(), getCanvasHeight());
			channelTwo->end();
		} else if(layout==ofxProjectorBlend_Vertical) {			
			channelTwo->begin();
			fullTexture->draw(0, -singleChannelHeight+pixelOverlap, getCanvasWidth(), getCanvasHeight());
			channelTwo->end();
		}
		
	}ofPopStyle();
}




void ofxProjectorBlend::draw()
{
	ofPushStyle();

	ofSetRectMode(OF_RECTMODE_CORNER);

	if(showBlend) {

		glActiveTexture(GL_TEXTURE0);	
		blendShader->begin();
		
		blendShader->setUniform1i("Tex0", 0);
		blendShader->setUniform1f("width", singleChannelWidth);
		blendShader->setUniform1f("height", singleChannelHeight);
		
		blendShader->setUniform1f("OverlapTop", 0.0f);
		blendShader->setUniform1f("OverlapLeft", 0.0f);
		blendShader->setUniform1f("OverlapBottom", 0.0f);
		blendShader->setUniform1f("OverlapRight", 0.0f);	
		
		if(layout==ofxProjectorBlend_Horizontal) {
			blendShader->setUniform1f("OverlapRight", pixelOverlap);	
		} else {
			blendShader->setUniform1f("OverlapTop", pixelOverlap);
		}
		
		blendShader->setUniform1f("BlackOutLeft", 0.0f);
		blendShader->setUniform1f("BlackOutRight", 0.0f);
		blendShader->setUniform1f("BlackOutTop", 0.0f);
		blendShader->setUniform1f("BlackOutBottom", 0.0f);
		
		blendShader->setUniform1f("BlendPower", blendPower);
		blendShader->setUniform1f("SomeLuminanceControl", luminance);
		blendShader->setUniform3f("GammaCorrection", gamma, gamma, gamma);
		blendShader->setUniform1f("SolidEdgeEnable", 0.0f);
		
		blendShader->setUniform4f("SolidEdgeColor", 0.0f, 0.0f, 0.0f, 1.0f);
		
				
		channelOne->draw(0, 0, singleChannelWidth, singleChannelHeight);
		
		
		if(layout==ofxProjectorBlend_Horizontal) {
			blendShader->setUniform1f("OverlapLeft", pixelOverlap);
			blendShader->setUniform1f("OverlapRight", 0.0f);
		} else {
			blendShader->setUniform1f("OverlapTop", 0.0f);
			blendShader->setUniform1f("OverlapBottom", pixelOverlap);
		}
		
		if(layout==ofxProjectorBlend_Horizontal) {
			channelTwo->draw(singleChannelWidth, 0, singleChannelWidth, singleChannelHeight);
		} else {
			channelTwo->draw(0, singleChannelHeight, singleChannelWidth, singleChannelHeight); 
		}
		
		blendShader->end();
	}
	else{
		channelOne->draw(0, 0, singleChannelWidth, singleChannelHeight);
		if(layout==ofxProjectorBlend_Horizontal) {
			channelTwo->draw(singleChannelWidth-pixelOverlap, 0, singleChannelWidth, singleChannelHeight);
		} else {
			channelTwo->draw(0, singleChannelHeight-pixelOverlap, singleChannelWidth, singleChannelHeight); 
		}
	}
	ofPopStyle();
}
