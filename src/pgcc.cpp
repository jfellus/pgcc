/*
 * pgcc.cpp
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#include "pgcc.h"



int VERBOSE = 0;

void compile_script(const std::string& script_file, const std::string& out_dir) {
	CPPWriter w;

	Script* main_script = add_script(read_script(script_file));
	w.set_main_script(main_script);

	if(VERBOSE) main_script->dump();

	w.write(out_dir);

	if(VERBOSE) DBG("DONE !");
}
