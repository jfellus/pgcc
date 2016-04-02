/*
 * Module.cpp
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#include "model.h"
#include "Module.h"

Module::Module(Script* s, const std::string& cls, const std::string& id) : script(s), cls(cls),id(id) {
	the_script = 0;
	bProcessable = true;
	used = 0;
	thread = 0;
	nb_no_async_ins = 0;
	if(str_starts_with(cls, "In<")) bProcessable = false;
}

void Module::dump() {
	std::cout << cls << " " << id << " (ins=[";
	for(uint i=0; i<ins.size(); i++) {
		if(i!=0) std::cout << ", ";
		std::cout << (ins[i]->src!=NULL ? ins[i]->src->id : "(null)");
	}
	std::cout << "], outs=[";
	for(uint i=0; i<outs.size(); i++) {
		if(i!=0) std::cout << ", ";
		std::cout << (outs[i]->dst!=NULL ? outs[i]->dst->id : "(null)");
	}
	std::cout << "])\n";
}
