/*
 *  Enableable.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

#pragma once

/**
 * Interface for elements you can enable and disable
 */
class Enableable {
public:
	Enableable() {
		enabled = false;
	}
	bool getEnabled() { return enabled; }
	virtual void setEnabled(bool enabled) { this->enabled = enabled; }
	
	void enable()  { setEnabled(true); }
	void disable() { setEnabled(false); }

	
	void toggle() {
		if(getEnabled()) disable();
		else enable();
	}
protected:
	bool enabled;
};