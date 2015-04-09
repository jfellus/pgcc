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


Module* Thread::get_timescale_start_module(Module* m) {
	Module* start_timescale = script->get_timescale_start_module(m);
	if(!start_timescale) {
		start_timescale = add(script->create_module("$START_TIMESCALE", TOSTRING("$START_TIMESCALE_" << m->timescale << "#" << m->get_thread_id())));
		start_timescale->timescale = m->timescale;
	}
	return start_timescale;
}

Module* Thread::get_timescale_end_module(Module* m) {
	Module* end_timescale = script->get_timescale_end_module(m);
	if(!end_timescale) {
		end_timescale = add(script->create_module("$END_TIMESCALE", TOSTRING("$END_TIMESCALE_" << m->timescale << "#" << m->get_thread_id())));
		end_timescale->timescale = m->timescale;
	}
	return end_timescale;
}


void Thread::compute_timescales() {
	for(uint i=0; i<modules.size(); i++) {
		Module* m = modules[i];
		if(m->timescale==0) continue;
		if(m->is_timescale()) continue;

		Module* start_timescale = get_timescale_start_module(m);
		Module* end_timescale = get_timescale_end_module(m);
		if(!end_timescale) end_timescale = add(script->create_module("$END_TIMESCALE", TOSTRING("$END_TIMESCALE_" << m->timescale << "#" << m->get_thread_id())));

		script->connect(start_timescale, m);
		script->connect(m, end_timescale);
	}
}
