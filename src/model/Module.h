/*
 * Module.h
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#ifndef MODULE_H_
#define MODULE_H_


#include "model.h"

class Module {
public:
	Script* script;
	std::string cls;
	std::string id;
	Script* the_script;

	array<Link*> outs;
	array<Link*> ins;
	std::map<std::string, std::string> params;

	Thread* thread;
	bool bProcessable;

public:
	uint used;
	uint nb_no_async_ins;
public:

	Module(Script* s, const std::string& cls, const std::string& id);

	void set_param(const std::string& key, const std::string& value) {
		params[key] = value;
	}

	bool use() {
		if(++used >= ins.size()) reset_use();
		else return false;
		return true;
	}
	bool use_no_async() {
		if(++used >= nb_no_async_ins) reset_use();
		else return false;
		return true;
	}


	void reset_use() { used = 0; }


	inline std::string get_decl_str() {
		return the_script ?  TOSTRING("Script_" << cls << " " << id << ";") :
							TOSTRING(cls << " " << id << ";");
	}

	inline int get_thread_id() { return thread ? thread->id : -1; }

	inline bool has_init() { return isalpha(cls[0]) && !is_timescale(); }
	inline bool is_processable() { return bProcessable; }
	inline bool needs_params() {return !the_script; }
	inline bool is_timescale() {return cls=="FOR" || cls=="ENDFOR";}

	inline std::string get_special_process_statement() {
		if(str_starts_with(cls, "_expr<")) {
			std::string type = str_between(cls, "<", ">");
			return TOSTRING(type << " " << id << " = " << params["_expr"] << ";");
		}
		return "";
	}

	void dump();

};




#endif /* MODULE_H_ */
