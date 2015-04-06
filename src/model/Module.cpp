/*
 * Module.cpp
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#include "model.h"
#include "Module.h"

Module::Module(const std::string& cls, const std::string& id) : cls(cls),id(id) {
	the_script = 0;
	bProcessable = true;
	used = 0;
	thread = 0;
	if(str_starts_with(cls, "In<")) bProcessable = false;
}
