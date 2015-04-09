/*
 * writer.h
 *
 *  Created on: 4 avr. 2015
 *      Author: jfellus
 */

#ifndef PG_WRITER_H_
#define PG_WRITER_H_

#include "../utils/utils.h"
#include "../model/model.h"

class CPPWriter {
public:
	Script* main_script;
public:
	CPPWriter() { main_script = NULL; }

	void set_main_script(Script* main_script) { this->main_script = main_script; }

	void write(const std::string& _dir) {
		if(!main_script) ERROR("No main_script specified !");

		std::string dir = _dir;
		if(dir.empty()) dir="./out";
		mkrmdir(dir);
		mkdir(TOSTRING(dir << "/utils"));
		mkdir(TOSTRING(dir << "/scripts"));

		// Copy core files
		SYSTEM("cp resources/sync_semaphores.h " << dir << "/utils");
		SYSTEM("cp resources/Makefile " << dir);
		SYSTEM("cp resources/README " << dir);

		// Create CPP files
		for(uint i=0; i<scripts.size(); i++) {
			write_cpp_script(scripts[i], TOSTRING(dir << "/scripts/" << scripts[i]->name << ".h"));
		}
		write_cpp_main(TOSTRING(dir << "/main.cpp"));
	}

	void write_cpp_script(Script* s, const std::string& filename) {
		std::ofstream f(filename.c_str(), std::ofstream::out);
		write_cpp_script(s, f);
		f.close();
	}

	void write_cpp_main(const std::string& filename) {
		std::ofstream f(filename.c_str(), std::ofstream::out);
		write_cpp_main(f);
		f.close();
	}


private:
	int NESTED_LEVEL = 0;

#include "writer_macros.h"

	void write_cpp_main(std::ofstream& f) {
		HEADER();
		________
		INCLUDE_SYS("pthread.h");
		INCLUDE_SYS("unistd.h");
		________
		for(uint i=0; i<scripts.size(); i++) INCLUDE("scripts/" << scripts[i]->name << ".h");
		________
		________
		f << "// The main script \n";
		f << "Script_" << main_script->name << " main_script;\n";
		________
		________
		COMMENT("THREADS");
		for(uint t=0; t<main_script->threads.size(); t++) {
			f << "void* f_thread_" << t << "(void*) { for(;;) main_script.process_thread_" << t << "(); return 0; }\n";
		}
		________
		________
		COMMENT("MAIN");
		FUNCTION(int, "main", (void));
		I(main_script.init(););
		for(uint t=0; t<main_script->threads.size(); t++)
			J("pthread_t th"<<t<<"; pthread_create(&th" << t << ", 0, f_thread_" << t << ", 0);");
		END_FUNCTION();
	}

	void write_cpp_script(Script* s, std::ofstream& f) {
		HEADER();
		f << "#ifndef __PG_SCRIPT_" << s->name << "_H\n";
		f << "#define __PG_SCRIPT_" << s->name << "_H\n";
		________
		write_script_includes(s, f);
		________
		________
		write_script_implementation(s, f);
		f << "#endif // __PG_SCRIPT_" << s->name << "_H\n";
	}

	void write_header(std::ofstream& f) {NESTED_LEVEL = 0; copy_file_to("resources/header.txt", f);}

	void write_script_includes(Script* s, std::ofstream& f) {
		COMMENT("INCLUDES");

		INCLUDE("../utils/sync_semaphores.h");

		for(uint i=0; i<s->depends.size(); i++)
			for(uint j=0; j<s->depends[i]->includes.size(); j++)
				INCLUDE(s->depends[i]->includes[j]);
		for(uint i=0; i<s->includes.size(); i++) INCLUDE(s->includes[i]);

		for(uint i=0; i<s->modules.size(); i++) {
			if(s->modules[i]->the_script && !str_has(s->modules[i]->id, "#"))
				INCLUDE(s->modules[i]->id << ".h");
		}
	}

	void write_script_implementation(Script* s, std::ofstream& f) {
		s->reset_modules_consumes();

		COMMENT("IMPLEMENTATION OF SCRIPT " << s->name);

		CLASS("Script_" << s->name);

		// Members
		for(uint i=0; i<s->modules.size(); i++) if(s->modules[i]->has_init()) J(s->modules[i]->get_decl_str());
		________
		write_semaphores_decls(s, f);
		________
		________

		// Constructor
		CONSTRUCTOR("Script_" << s->name, ());
		for(uint i=0; i<s->modules.size(); i++) {
			for(std::map<std::string, std::string>::iterator j = s->modules[i]->params.begin(); j!=s->modules[i]->params.end(); j++) {
				JJ(s->modules[i]->id << "." << (*j).first << " = " << (*j).second << ";");
			}
		}
		END_CONSTRUCTOR();

		// Init
		METHOD(void, "init", ());
		std::list<Module*> bfs;
		for(uint i=0;i<s->root_modules.size(); i++) bfs.push_back(s->root_modules[i]);

		while(!bfs.empty()) {
			Module* m = bfs.front(); bfs.pop_front();
			if(m->use()) {
				for(uint i=0; i<m->outs.size(); i++) {
					bfs.push_back(m->outs[i]->dst);
					if(m->outs[i]->is_sync()) {
						JJ("SEM_INIT(" << SM(m->id) << ", " << SM(m->outs[i]->dst->id) << ");");
					}
				}
				if(m->has_init())  JJ(m->id << "." << "init();");
			}
		}

		END_METHOD();

		________
		// Threads methods
		COMMENT("THREADS (" << s->threads.size() << ")");
		for(uint t=0; t<s->threads.size(); t++) {
			METHOD(void, "process_thread_" << t, ());
				write_thread(f, s->threads[t]);
			END_METHOD();
		}

		END_CLASS();
	}

	void write_thread(std::ofstream& f, Thread* thread) {
		thread->script->reset_use();
		std::list<Link*> bfs;
		for(uint i=0;i<thread->script->root_modules.size(); i++) bfs.push_back(new Link(NULL, "", thread->script->root_modules[i],"", ""));

		while(!bfs.empty()) {
			Link* l = bfs.front(); bfs.pop_front();
			if(l->dst->use()) {
				for(uint i=0; i<l->dst->outs.size(); i++) bfs.push_back(l->dst->outs[i]);
				if(l->dst->thread==thread) {
					if(l->dst->the_script) write_process_script(f, l);
					else write_process_module(f, l);
				}
			}
			if(l->src==NULL) { delete l; l = 0; }
		}
		thread->script->reset_use();
	}

	void write_process_module(std::ofstream& f, Link* l) {
		Module* m = l->dst;
		if(m->cls=="$START_TIMESCALE") {write_process_start_timescale(f, l); return; }
		else if(m->cls=="$END_TIMESCALE") {write_process_end_timescale(f, l); return; }
		std::string process = "process";
		std::string id = m->id;
		if(str_has(id, "#")) {
			process = TOSTRING("process_thread_" << str_after(id, "#"));
			id = str_before(id, "#");
		}

		// Handle sync input links
		for(uint i=0; i<m->ins.size(); i++) {
			Link* l = m->ins[i];
			if(!l->is_sync()) continue;
			JJ("SEM_RECV(" << SM(l->src->id) << ", " << SM(m->id) << ");");
		}

		if(!m->is_processable()) return;

		// Call process()
		f << REPEAT_STR("\t", NESTED_LEVEL) << id << (!l->dst_pin.empty() ? "." : "") << l->dst_pin << "." << process << "(";
		if(l->src) {
			for(uint i=0; i<m->ins.size(); i++) {
				if(m->ins[i]->is_timescale_link()) continue;
				if(i!=0) f << ", ";
				f << m->ins[i]->src->id;
				if(!m->ins[i]->src_pin.empty()) f << "." << m->ins[i]->src_pin;
			}
		}
		f << ");\n";

		// Handle sync output links
		for(uint i=0; i<m->outs.size(); i++) {
			Link* l = m->outs[i];
			if(!l->is_sync()) continue;
			JJ("SEM_SEND(" << SM(m->id) << ", " << SM(l->dst->id) << ");");
		}
	}

	void write_process_script(std::ofstream& f, Link* l) {
		Module* m = l->dst;
		if(!m->is_processable()) return;
		if(!str_has(m->id, "#")) return;
		uint t = TOINT(str_after(m->id, "#"));
		Thread* thread = m->the_script->threads[t];
		std::string id = str_before(m->id, "#");

		if(!thread->isConsumed) {
			thread->consume();

			// Handle sync input links
			for(uint i=0; i<m->ins.size(); i++) {
				Link* l = m->ins[i];
				if(!l->is_sync()) continue;
				JJ("SEM_RECV(" << SM(l->src->id) << ", " << SM(id) << ");");
			}

			// Call process_thread_xxx()
			JJ(id << ".process_thread_" <<  t << "();");

			// Handle sync output links
			for(uint i=0; i<m->outs.size(); i++) {
				Link* l = m->outs[i];
				if(!l->is_sync()) continue;
				JJ("SEM_SEND(" << SM(id) << ", " << SM(l->dst->id) << ");");
			}
		}
	}

	void write_process_start_timescale(std::ofstream& f, Link* l) {
		std::string v; v += (char)('i'+NESTED_LEVEL);
		int nb_iterations = l->dst->script->get_timescale_iterations(l->dst->timescale);
		JJ("for (int " << v << "=0; " << v << "<" << nb_iterations << "; " << v << "++) {");
		NESTED_LEVEL++;
	}

	void write_process_end_timescale(std::ofstream& f, Link* l) {
		NESTED_LEVEL--;
		JJ("}");
	}

	void write_semaphores_decls(Script* s, std::ofstream &f) {
		for(uint i=0; i<s->modules.size(); i++) {
			for(uint j=0; j<s->modules[i]->outs.size(); j++) {
				Link* l = s->modules[i]->outs[j];
				if(!l->is_sync()) continue;
				J("SEM_DECL(" << SM(l->src->id) << ", " << SM(l->dst->id) << ");");
			}
		}
	}

	inline std::string SM(const std::string& id) {	return str_replace(str_before(id, "#"), ".", "_");}

#include "writer_macros.h"

};





#endif /* PG_WRITER_H_ */
