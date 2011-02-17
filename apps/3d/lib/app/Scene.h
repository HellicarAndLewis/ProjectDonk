#pragma once

#include "Projector.h"
#include "Model.h"
#include "ofScene3d.h"

/**
 * \brief Represents the real world setup of physical surfaces, 
 * and the projectors in the scene
 */
class Scene: public ofScene3d {
public:
	/** Loads the scene from XML */
	Scene();
	
	~Scene();
	/**
	 * Returns the model of the scene
	 */
	Model *getModel();
	
	void update();
	
	vector<Projector*> projectors;
	
private:
	
	Model *model;
};