/*
 * model.cpp
 *
 *  Created on: 4 avr. 2015
 *      Author: jfellus
 */

#include "model.h"
#include "../io/reader.h"

array<Script*> scripts;
array<Project*> projects;

Script* read_script(const std::string& filename) {
	ScriptReader r; return r.read_script(filename);
}

Script* add_script(Script* s) {
	scripts.add(s);
	return s;
}
