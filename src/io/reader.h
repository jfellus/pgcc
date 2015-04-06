/*
 * reader.h
 *
 *  Created on: 5 avr. 2015
 *      Author: jfellus
 */

#ifndef READER_H_
#define READER_H_

#include "../utils/utils.h"

class ScriptReader {
public:
	std::string filename;
	Script* script;
	Module* module;
	Link* link;

public:
	ScriptReader() {script = 0; module = 0; link = 0;}

	Script* read_script(const std::string& filename) {
		this->filename = filename;
		std::ifstream f(filename);
		Script* s = read_script(f);
		f.close();
		script = 0; module = 0; link = 0; this->filename = "";
		return s;
	}

	Script* read_script(std::ifstream& f) {
		std::string line;
		std::string section = "header";
		while (std::getline(f, line)) {
			line = str_trim(str_before(line, "#"));
			if(line.empty()) continue;
			if(line[0]=='[') section = str_to_lower(str_trim(str_between(line, "[", "]")));
			else if(section=="header") read_header_statement(line);
			else if(section=="modules") read_module_statement(line);
			else if(section=="links") read_links_statement(line);
		}

		script->compute_threads();

		return script;
	}

	void read_header_statement(const std::string& line) {
		if(str_starts_with(line, "Script")) script = new Script(filename, str_trim(str_after(line, "Script")));
		else if(str_starts_with(line, "Include")) script->add_include(str_trim(str_after(line, "Include")));
		else if(str_starts_with(line, "Depends")) script->add_depend(str_trim(str_after(line, "Depends")));
	}

	void read_module_statement(const std::string& line) {
		if(str_has(line, "=")) module->set_param(str_trim(str_before(line, "=")), str_trim(str_after(line, "=")));
		else module = create_module(str_trim(line));
	}

	void read_links_statement(const std::string& line) {
		if(str_has(line, "=")) {
			std::string key = str_trim(str_before(line, "="));
			std::string val = str_trim(str_after(line, "="));
			if(key=="type") link->type = val;
		} else link = create_link(str_trim(line));
	}

	Module* create_module(const std::string& _spec) {
		std::string spec = _spec;
		bool bIsScript = spec[0]=='$';
		if(bIsScript) spec = spec.substr(1);
		std::string cls = str_trim(str_before(spec, " "));
		std::string id = str_trim(str_after(spec, " "));
		if(!bIsScript) return script->create_module(cls,id);
		else return script->create_module_script(cls,id);
	}

	Link* create_link(const std::string& spec) {
		std::string ssrc = str_trim(str_before(spec, "-"));
		std::string sdst = str_trim(str_after(spec, ">"));
		std::string stype = str_trim(str_between(spec, "-", ">"));
		std::string srcpin = "";
		std::string dstpin = "";
		if(str_has(ssrc, ".")) { srcpin = str_trim(str_after(ssrc, ".")); ssrc = str_trim(str_before(ssrc, ".")); }
		if(str_has(sdst, ".")) { dstpin = str_trim(str_after(sdst, ".")); sdst = str_trim(str_before(sdst, ".")); }
		Module* src = script->get_module(ssrc);
		Module* dst = script->get_module(sdst);
		return script->connect(src,srcpin,dst,dstpin, stype);
	}

};






#endif /* READER_H_ */
