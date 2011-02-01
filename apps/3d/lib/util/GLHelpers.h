/*
 *  GLHelpers.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 17/01/2011.
 *
 */

#pragma once
#include "ofMain.h"
namespace util {
	
/**
 * \brief This calls glEnable/disable while the object is in scope, 
 * and then returns openGL to its previous state when the destructor is called.
 * 
 * e.g. 
 * {
 *		// enable depth test
 *		ScopedGLCapability depth(GL_DEPTH_TEST, true);
 *
 *		// do some stuff
 * }
 * // depth is out of scope, so GL_DEPTH_TEST should be reverted to state previously set.
 */
class ScopedGLCapability {
public:
	
	bool wasEnabled;
	bool shouldEnable;
	int glCapability;
	
	ScopedGLCapability(int glCapability, bool shouldEnable) {
		
		this->glCapability = glCapability;
		this->wasEnabled = glIsEnabled(glCapability);
		this->shouldEnable = shouldEnable;
		
		if(shouldEnable!=wasEnabled) {
			if(shouldEnable) glEnable(glCapability);
			else glDisable(glCapability);
		}
	}
	
	~ScopedGLCapability() {
		if(shouldEnable!=wasEnabled) {
			if(wasEnabled) glEnable(glCapability);
			else glDisable(glCapability);
		}
	}
};
};