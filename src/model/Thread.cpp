/*
 * Thread.cpp
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#include "model.h"
#include "Thread.h"

Thread::Thread(Script* script, uint id) : script(script), id(id) {
	nbProcessables = 0;
	isConsumed = false;
}

Thread::~Thread() {
	for(uint i=0; i<modules.size(); i++) modules[i]->thread = NULL;
}

Module* Thread::add(Module* m) {
	m->thread = this;
	modules.add(m);
	if(m->is_processable()) nbProcessables++;

	bool bIsroot = true;
	for(uint i=0; i<m->ins.size(); i++) if(m->ins[i]->src->thread==this) bIsroot = false;
	if(bIsroot) root_modules.add(m);

	for(uint i=0; i<m->outs.size(); i++) if(m->outs[i]->dst->thread==this) root_modules.remove(m->outs[i]->dst);
	return m;
}


