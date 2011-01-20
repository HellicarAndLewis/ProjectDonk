#pragma once

#include "Kinect.h"
#include "Projector.h"
#include "Model.h"

/**
 * \brief Represents the real world setup of physical surfaces, 
 * the projectors in the scene, and the Kinects.
 */
class Scene {
public:
	/** Loads the scene from XML */
	Scene();
	
	/**
	 * Returns the model of the scene
	 */
	Model *getModel();
	
	void update();
	
	vector<Projector*> projectors;
	vector<Kinect*>    kinects;
	
private:
	
	Model *model;
};