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
#include "../model/ModulesPrototypes.h"

class CPPWriter {
public:
	Script* main_script;
	ModulesPrototypes* modules_prototypes;

public:
	CPPWriter() { NESTED_LEVEL = 0; main_script = NULL; modules_prototypes = 0;}

	void set_main_script(Script* main_script) { this->main_script = main_script; }

	void write(const std::string& _dir) {
		if(!main_script) ERROR("No main_script specified !");

		std::string dir = _dir;
		if(dir.empty()) dir="./out";
		mkrmdir(dir);
		mkdir(TOSTRING(dir << "/utils"));
		mkdir(TOSTRING(dir << "/scripts"));

		// Copy core files
		SYSTEM("cp -f resources/sync_semaphores.h " << dir << "/utils");
		SYSTEM("cp -f resources/README " << dir);

		// Create Makefile
		create_makefile(TOSTRING(dir << "/Makefile"), "resources/Makefile");

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

#include "writer_macros.h"
	int NESTED_LEVEL;

	void create_makefile(const std::string& outfile, const std::string& src) {
		std::ofstream f(outfile.c_str());

		f << "LD_FLAGS:=";
		for(uint i=0; i<scripts.size(); i++)
			for(uint j=0; j<scripts[i]->depends.size(); j++)
				f << scripts[i]->depends[j]->get_ld_str();
		f << "\n";
		f << "CFLAGS:=";
		for(uint i=0; i<scripts.size(); i++)
			for(uint j=0; j<scripts[i]->depends.size(); j++)
				f << scripts[i]->depends[j]->get_cflags_str();
		f << "\n";

		copy_file_to(src, f);

		f.close();
	}

	void write_cpp_main(std::ofstream& f) {
		HEADER();
		________
		INCLUDE_SYS("pthread.h");
		INCLUDE_SYS("unistd.h");
		INCLUDE_SYS("pg.h");
		INCLUDE_SYS("utils/utils.h");
		INCLUDE("utils/sync_semaphores.h");
		f << "#ifdef DEBUG\n";
		INCLUDE_SYS("pgdb.h");
		f << "#endif\n";
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
			f << "void* f_thread_" << t << "(void*) { try { while(bSIMULATION_RUNNING) main_script.process_thread_" << t << "(); } catch(std::runtime_error& x) { PRINT_FATAL_ERROR(x.what()); } return 0;}\n";
		}
		________
		________
		COMMENT("MAIN");
		FUNCTION(int, "main", (void));
		I(REGISTER_ATEXIT(););
		I(main_script.init(););
		f << "#ifdef DEBUG\n";
		J("pgdb_init_script(NULL, &main_script, \"main_expe\");");
		I(main_script.init_debug(););
		J("pgdb_start();");
		f << "#endif\n";
		for(uint t=0; t<main_script->threads.size(); t++)
			J("pthread_t th"<<t<<"; pthread_create(&th" << t << ", 0, f_thread_" << t << ", 0);");
		for(uint t=0; t<main_script->threads.size(); t++) J("pthread_join(th"<<t<<",0);");
		J("DBG(\"Bye bye !\");");
		END_FUNCTION();
	}

	void write_cpp_script(Script* s, std::ofstream& f) {
		HEADER();
		f << "#ifndef __PGCC_SCRIPT_" << s->name << "_H\n";
		f << "#define __PGCC_SCRIPT_" << s->name << "_H\n";
		________
		write_script_includes(s, f);
		________
		________
		write_script_implementation(s, f);
		f << "#endif // __PGCC_SCRIPT_" << s->name << "_H\n";
	}

	void write_header(std::ofstream& f) {NESTED_LEVEL = 0; copy_file_to("resources/header.txt", f);}

	void write_script_includes(Script* s, std::ofstream& f) {
		COMMENT("INCLUDES");
		INCLUDE("../utils/sync_semaphores.h");
		INCLUDE_SYS("pg.h");
		f << "#ifdef DEBUG\n";
		INCLUDE_SYS("pgdb.h");
		f << "#endif\n";

		for(uint i=0; i<s->depends.size(); i++)
			for(uint j=0; j<s->depends[i]->includes.size(); j++)
				INCLUDE(s->depends[i]->includes[j]);
		for(uint i=0; i<s->includes.size(); i++) INCLUDE(s->includes[i]);

		for(uint i=0; i<s->modules.size(); i++) {
			if(s->modules[i]->the_script && !str_has(s->modules[i]->id, "#"))
				INCLUDE(s->modules[i]->cls << ".h");
		}
	}

	void write_script_implementation(Script* s, std::ofstream& f) {
		s->reset_modules_consumes();

		COMMENT("IMPLEMENTATION OF SCRIPT " << s->name);

		CLASS("Script_" << s->name);

		// Members
		f << "#ifdef DEBUG\n";
		for(uint i=0; i<s->modules.size(); i++) if(s->modules[i]->has_init() && !s->modules[i]->the_script) J("ModulePlugs* __debug_plugs__" << s->modules[i]->id << ";");
		f << "#endif\n";
		________
		for(uint i=0; i<s->modules.size(); i++) if(s->modules[i]->has_init()) J(s->modules[i]->get_decl_str());
		________
		write_semaphores_decls(s, f);
		________
		________

		// Constructor
		CONSTRUCTOR("Script_" << s->name, ());
		for(uint i=0; i<s->modules.size(); i++) {
			if(s->modules[i]->is_timescale()) continue;
			for(std::map<std::string, std::string>::iterator j = s->modules[i]->params.begin(); j!=s->modules[i]->params.end(); j++) {
				if(isalpha((*j).first[0]))
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
			if(m->use_no_async()) {
				for(uint i=0; i<m->outs.size(); i++) {
					if(!m->outs[i]->is_async()) bfs.push_back(m->outs[i]->dst);
					if(m->outs[i]->is_sync()) {
						JJ("SEM_INIT(" << SM(m->id) << ", " << SM(m->outs[i]->dst->id) << ");");
					}
				}
				if(m->has_init())  JJ(m->id << "." << "init();");
			}
		}

		END_METHOD();

		________

		// Init debug Plugs
		f << "#ifdef DEBUG\n";
		METHOD(void, "init_debug", ());
		for(uint i=0; i<s->modules.size(); i++) {
			if(str_has(s->modules[i]->id, "#")) continue;
			if(s->modules[i]->is_timescale()) continue;
			if(str_has(s->modules[i]->id, ".")) continue;
			if(s->modules[i]->the_script)
				 JJ("pgdb_init_script(this, &" << s->modules[i]->id << ",\"" << s->modules[i]->id << "\");");
			else JJ("__debug_plugs__" << s->modules[i]->id << " = pgdb_init_module(this, &" << s->modules[i]->id << ",\"" << s->modules[i]->id << "\");");
			if(s->modules[i]->the_script) {
				JJ(s->modules[i]->id << ".init_debug();");
			}
		}
		END_METHOD();
		f << "#endif\n";
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
		for(uint i=0;i<thread->root_modules.size(); i++) {
			if(thread->root_modules[i]->thread!=thread) continue;
			bfs.push_back(new Link(NULL, "", thread->root_modules[i],"", ""));
		}

		while(!bfs.empty()) {
			Link* l = bfs.front(); bfs.pop_front();
			if(l->dst->use_no_async()) {
				for(uint i=0; i<l->dst->outs.size(); i++) if(!l->dst->outs[i]->is_async()) bfs.push_back(l->dst->outs[i]);
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
		if(m->cls=="FOR") {write_FOR(f, l); return; }
		else if(m->cls=="ENDFOR") {write_ENDFOR(f, l); return; }

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

		std::string special = m->get_special_process_statement();
		if(!special.empty()) { JJ(special); return; }


		// Call process()
		f << REPEAT_STR("\t", NESTED_LEVEL) << id << (!l->dst_pin.empty() ? "." : "") << l->dst_pin << "." << process << "(";
		if(l->src) write_process_module_args(f, m);
		f << ");";

		// Debug : send plug data
		if(!str_has(m->id, ".")) JJ("    ___dbs(__debug_plugs__" << m->id << ")"); else f << "\n";

		// Handle sync output links
		for(uint i=0; i<m->outs.size(); i++) {
			Link* l = m->outs[i];
			if(!l->is_sync()) continue;
			JJ("SEM_SEND(" << SM(m->id) << ", " << SM(l->dst->id) << ");");
		}
	}

	void write_process_module_args(std::ofstream& f, Module* m) {
		std::string args = "";
		if(modules_prototypes) args = modules_prototypes->compute_params_str(m);
		if(args.empty()) {
			for(uint i=0; i<m->ins.size(); i++) {
				if(!m->ins[i]->has_data()) continue;
				if(i!=0) f << ", ";
				f << m->ins[i]->src->id;
				if(!m->ins[i]->src_pin.empty()) f << "." << m->ins[i]->src_pin;
			}
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

	void write_FOR(std::ofstream& f, Link* l) {
		if(l->dst->params.count("iterations")) {
			std::string nb_iterations = l->dst->params["iterations"];
			JJ("for (int " << l->dst->id << "=0; " << l->dst->id << "<" << nb_iterations << "; " << l->dst->id << "++) {");
			NESTED_LEVEL++;
		}
	}

	void write_ENDFOR(std::ofstream& f, Link* l) {
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


	void read_prototypes(const std::string& outfile) {
		if(modules_prototypes) return;

		// Write fake CPP main and fake script
		std::string fakecpp = TOSTRING(str_dirname(outfile) << "/fake.cpp");
		std::string fakeh = TOSTRING(str_dirname(outfile) << "/scripts/fake.h");
		std::ofstream ffakecpp(fakecpp.c_str()), ffakeh(fakeh.c_str());
		for(uint i=0; i<scripts.size(); i++) write_script_includes(scripts[i], ffakeh);
		ffakecpp << "#include \"scripts/fake.h\"\nvoid main() {}\n";
		ffakecpp.close();
		ffakeh.close();

		// List process() methods prototypes
		SYSTEM("./resources/list_process_prototypes.sh " << outfile << " fake.cpp -I~/.pgcc/includes");

		modules_prototypes = new ModulesPrototypes();

		std::string line;
		std::ifstream f(outfile.c_str());
		std::string cls;
		while(getline(f,line)) {
			if(str_starts_with(line, "class")) cls = str_trim(str_after(line, "class"));
			else if(line == "process") modules_prototypes->create_process(cls);
			else modules_prototypes->create_param(cls, line);
		}
		f.close();

		SYSTEM("rm -f " << fakecpp << " " << fakeh);
	}

#include "writer_macros.h"

};





#endif /* PG_WRITER_H_ */
