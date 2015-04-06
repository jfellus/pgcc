/*
 * pgcc.cpp
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#include "pgcc.h"



void compile_script(const std::string& script_file, const std::string& out_dir) {
	ScriptReader r;
	CPPWriter w;

	Script* main_script = r.read_script(script_file);
	w.set_main_script(main_script);

//	main_script->dump();

	w.write(out_dir);

//	DBG("DONE !");
}
