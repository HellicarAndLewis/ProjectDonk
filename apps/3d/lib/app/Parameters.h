/**
 * parameters.h
 * Render
 *
 * Created by Marek Bereza on 21/02/2011.
 *
 */
#pragma once
#include "ofMain.h"

class Parameters: public vector<string> {
public:
	Parameters() {
		push_back("one");
		push_back("two");
		push_back("three");
	}
};
