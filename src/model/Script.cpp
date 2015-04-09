/*
 * Script.cpp
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#include "model.h"
#include "Script.h"


Module* Script::create_module(const std::string& cls, const std::string& id) {
	return add_module(new Module(this, cls, id));
}

Module* Script::create_module_script(const std::string& _filename, const std::string& id) {
	std::string filename = _filename;
	if(!file_exists(filename)) filename = TOSTRING(str_dirname(this->filename) << "/" << filename);
	if(!file_exists(filename)) ERROR("Can't find script file " << filename);
	Script* s = add_script(read_script(filename));
	for(uint t=0; t<s->threads.size(); t++) {
		Module* m = create_module("...", TOSTRING(id << "#" << t));
		m->the_script = s;
	}
	Module* m = create_module(s->name, id);
	m->the_script = s;
	m->bProcessable = false;
	return m;
}

void Script::dump() {
	DBG("---------- DUMP SCRIPT " << name << " ---------");
	reset_use();
	std::list<Link*> bfs;
	for(uint j=0; j<root_modules.size(); j++) bfs.push_back(new Link(NULL, "", root_modules[j], "", ""));
	while(!bfs.empty()) {
		Link* l = bfs.front(); bfs.pop_front();
		Module* m = l->dst;
		if(m->use()) {
			for(uint i=0; i<m->outs.size(); i++) bfs.push_back(m->outs[i]);

			std::cout << "Module " << m->cls << " " << m->id << " [" << m->get_thread_id() << "]\n";
			for(uint i=0; i<m->outs.size(); i++) {
				std::cout << "   -" << m->outs[i]->type << "> " << m->outs[i]->dst->id <<  "\n";
			}
		}
		if(l->src==NULL) delete l;
	}
	reset_use();
}
