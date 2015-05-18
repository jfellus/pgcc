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
	std::string target;
	Script* script;
	Module* module;
	Link* link;

public:
	ScriptReader() {script = 0; module = 0; link = 0;}


	/////////////
	// READING //
	/////////////

	Script* read_script(const std::string& filename, const std::string& target = "") {
		this->filename = filename;
		this->target = " " + target + " ";
		std::ifstream f(filename.c_str());
		if(!f.good()) ERROR("No such script file : " << filename);
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
		script->compute_timescales();

		return script;
	}


	/////////////////////
	// READ STATEMENTS //
	/////////////////////

	void read_header_statement(const std::string& line) {
		if(str_starts_with(line, "Script")) script = new Script(filename, str_trim(str_after(line, "Script")));
		else if(str_starts_with(line, "Include")) script->add_include(str_trim(str_after(line, "Include")));
		else if(str_starts_with(line, "Depends")) script->add_depend(str_trim(str_after(line, "Depends")));
		else if(str_starts_with(line, "Timescale")) {
			std::string params = str_trim(str_after(line, "Timescale"));
			int id = TOINT(str_trim(str_before(params, " ")));
			size_t nb = TOINT(str_trim(str_before(str_after(params, " "), " ")).substr(1));
			int parent = TOINT(str_trim(str_after(str_after(params, " "), " ")));
			script->add_timescale(id, nb, parent);
		}
	}

	void read_module_statement(const std::string& line) {
		if(str_has(line, "=")) {
			if(!module) return;
			module->set_param(str_trim(str_before(line, "=")), str_trim(str_after(line, "=")));
		}
		else module = create_module(str_trim(line));
	}

	void read_links_statement(const std::string& line) {
		if(str_has(line, "=") ) {
			if(!link) return;
			std::string key = str_trim(str_before(line, "="));
			std::string val = str_trim(str_after(line, "="));
			if(key=="type") link->type = val;
		} else link = create_link(str_trim(line));
	}


	////////////////////////
	// EXECUTE STATEMENTS //
	////////////////////////

	bool fit_target(const std::string& target) {
		if(target.empty() || str_trim(this->target).empty()) return true;

		std::string tok;
		if(target[0]=='+') {
			for(std::string rest = str_trim(target.substr(1));!str_trim(rest).empty();rest = str_trim(str_after(rest, " "))) {
				tok = str_trim(str_before(rest, " "));
				if(str_has(this->target,tok)) return true;
			}
			return false;
		} else if(target[0]=='-') {
			for(std::string rest = str_trim(target.substr(1));!str_trim(rest).empty();rest = str_trim(str_after(rest, " "))) {
				tok = str_trim(str_before(rest, " "));
				if(str_has(this->target,tok)) return false;
			}
			return true;
		}

		return false;
	}

	Module* create_module(const std::string& _spec) {
		std::string spec = _spec;
		bool bIsScript = spec[0]=='$';
		if(bIsScript) spec = spec.substr(1);

		std::string cls = str_trim(str_before(spec, " "));
		std::string id = str_trim(str_after(spec, " "));

		if(str_has(id, " ")) {
			std::string target = str_trim(str_after(id, " "));
			id = str_trim(str_before(id, " "));
			if(!fit_target(target)) return NULL;
		}

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
		if(!src || !dst) return NULL;
//		if(!src) ERROR("No such source module : " << ssrc);
//		if(!dst) ERROR("No such target module : " << sdst);

		Link* l = script->connect(src,srcpin,dst,dstpin, stype);

		return l;
	}

};






#endif /* READER_H_ */
