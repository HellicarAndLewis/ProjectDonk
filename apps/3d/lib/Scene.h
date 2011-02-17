
/**
 * Represents the real world setup of physical surfaces, 
 * the projectors in the scene, and the Kinects.
 */
#include "Kinect.h"
#include "Projector.h"
#include "Model.h"

class Scene {
private:
	vector<Projector*> projectors;
	Model *model;
};