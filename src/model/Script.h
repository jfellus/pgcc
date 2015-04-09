/*
 * Script.h
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#ifndef SCRIPT_H_
#define SCRIPT_H_



extern array<Script*> scripts;
class Script {
public:
	std::string filename;

	array<std::string> includes;
	array<Project*> depends;

	array<Module*> modules;
	array<Link*> links;
	array<Module*> root_modules;
	std::string name;


	//////////////////
	// CONSTRUCTION //
	//////////////////

	Script(const std::string& filename, const std::string& name) : filename(filename), name(name) {}


	///////////////
	// ACCESSORS //
	///////////////

	void add_include(const std::string& s) {includes.add(s);}
	void add_depend(const std::string& project) {	depends.add(get_project(project));	}
	void add_timescale(int id, size_t nb_iterations, int parent = 0) {
		timescales_iterations[id] = nb_iterations;
		timescales_parent[id] = parent;
	}

	Module* get_module(const std::string& id) {
		for(size_t i = 0; i<modules.size(); i++) if(modules[i]->id==id) return modules[i];
		return NULL;
	}


	//////////////////
	// MANIPULATION //
	//////////////////

	Link* connect(Module* src, const std::string& _src_pin, Module* dst, const std::string& _dst_pin, const std::string& _link_type) {
		std::string src_pin = _src_pin;
		std::string dst_pin = _dst_pin;
		std::string link_type = _link_type;

		if(dst->the_script && !dst_pin.empty()) {
			Module* dummy_module = create_module("...", TOSTRING(dst->id << "." << dst_pin));
			connect(src,src_pin, dummy_module, "", "");
			Module* pin = dst->the_script->get_module(dst_pin);
			if(!pin) ERROR("No such IN pin \"" << dst_pin << " in script " << dst->the_script->name);
			uint dst_thread = pin->thread->id;

			src = dummy_module;
			src_pin = dst_pin = "";
			dst = get_module(TOSTRING(dst->id << "#" << dst_thread));
		}
		else if(src->the_script && !src_pin.empty()) {
			Module* dummy_module = create_module("...", TOSTRING(src->id << "." << src_pin));
			dummy_module->bProcessable = false;
			Module* pin = src->the_script->get_module(src_pin);
			if(!pin) ERROR("No such OUT pin \"" << src_pin << " in script " << dst->the_script->name);
			uint src_thread = pin->thread->id;
			connect(get_module(TOSTRING(src->id << "#" << src_thread)), "", dummy_module, "", link_type);

			src = dummy_module;
			src_pin = "";
			link_type = "";
		}
		Link* l = new Link(src,src_pin,dst,dst_pin,link_type);
		links.add(l);
		root_modules.remove(dst);
		return l;
	}

	inline Link* connect(Module* src, Module* dst, const std::string& type = "") { return connect(src,"",dst,"",type); }
	inline Link* connect(Module* src, const std::string& src_pin, Module* dst, const std::string& type = "") { return connect(src,src_pin,dst,"",type); }
	inline Link* connect(Module* src, Module* dst, const std::string& dst_pin, const std::string& type) { return connect(src,"",dst,dst_pin,type); }

	Module* add_module(Module* m) { root_modules.add(m); modules.add(m); m->script = this; return m; }
	void remove_module(Module* m) { root_modules.remove(m); modules.remove(m);  m->script = 0;  }
	Module* create_module(const std::string& cls, const std::string& id);
	Module* create_module_script(const std::string& filename, const std::string& id);


	/////////////
	// THREADS //
	/////////////

	array<Thread*> threads;

	void compute_threads() {
		uint n=0;

		for(uint i=0; i<modules.size(); i++) modules[i]->thread = 0;
		reset_modules_consumes();

		Thread* thread = 0;
		for(uint c=0; c<modules.size(); c++) {
			if(thread && thread->nbProcessables==0) {threads.remove(thread); delete thread; thread = 0;}
			std::list<Module*> bfs;
			for(uint j=0; j<modules.size(); j++) if(modules[j]->thread==0 && modules[j]->is_processable()) {bfs.push_back(modules[j]); break;}
			if(!bfs.empty()) thread = threads.add(new Thread(this, threads.size()));
			while(!bfs.empty()) {
				Module* m = bfs.front(); bfs.pop_front();
				if(m->thread!=thread) {thread->add(m); n++;}
				if(n>=modules.size()) {
					if(thread && thread->nbProcessables==0) {threads.remove(thread); delete thread; thread = 0;}
					return;
				}
				for(uint i=0; i<m->ins.size(); i++) {
					if(!m->ins[i]->is_same_thread()) continue;
					if(m->ins[i]->src->thread==0) bfs.push_back(m->ins[i]->src);
				}
				for(uint i=0; i<m->outs.size(); i++) {
					if(!m->outs[i]->is_same_thread()) continue;
					if(m->outs[i]->dst->thread==0) bfs.push_back(m->outs[i]->dst);
				}
			}
		}
	}

	void dump_threads() {
		for(uint t=0; t<threads.size(); t++) {
			DBG("Thread " << threads[t]->id << " has " << threads[t]->modules.size() << " modules");
			std::cout << "[";
			for(uint i=0; i<threads[t]->modules.size(); i++) {
				if(i!=0) std::cout << " ";
				std::cout << threads[t]->modules[i]->id;
			}
			std::cout << "]\n";
		}
	}



	////////////////
	// TIMESCALES //
	////////////////

private:
	std::map<int,int> timescales_parent;
	std::map<int,size_t> timescales_iterations;
public:

	void timescale_set_parent(int ts_child, int ts_parent) {timescales_parent[ts_child] = ts_parent;}

	bool is_timescale_child(int ts1, int ts2) {
		int t = ts1;
		while(timescales_parent.count(t)) {
			t = timescales_parent[t];
			if(t==ts2) return true;
		}
		return false;
	}

	size_t get_timescale_iterations(int ts) { return timescales_iterations[ts]; }

	inline Module* get_timescale_start_module(Module* m) { return get_module(TOSTRING("$START_TIMESCALE_" << m->timescale << "#" << m->get_thread_id())); }
	inline Module* get_timescale_end_module(Module* m) { return get_module(TOSTRING("$END_TIMESCALE_" << m->timescale << "#" << m->get_thread_id())); }

	void compute_timescales() {
		for(uint t=0; t<threads.size(); t++) threads[t]->compute_timescales();
		for(uint i=0; i<links.size(); i++) {
			Link* l = links[i];
			if(l->src->timescale == l->dst->timescale) continue;
			if(!l->src->thread || !l->dst->thread) continue;
			if(l->is_timescale_link()) continue;
			if(is_timescale_child(l->dst->timescale,l->src->timescale)) {
				Module* ts_start = get_timescale_start_module(l->dst);
				Link* link = connect(l->src, ts_start, l->type);
				link->bNoData = true;
			} else {
				Module* ts_end = get_timescale_end_module(l->src);
				Link* link = connect(ts_end, l->dst, l->type);
				link->bNoData = true;
			}
		}
	}


	/////////////////
	// CONSUMPTION //
	/////////////////

	inline void reset_modules_consumes() {
		for(uint i=0; i<modules.size(); i++) if(modules[i]->the_script) modules[i]->the_script->reset_consume();
	}
	inline void reset_consume() {for(uint i=0; i<threads.size(); i++) threads[i]->reset_consume();}
	inline void reset_use() {for(uint i=0; i<modules.size(); i++) modules[i]->reset_use();}
	void dump();
};



#endif /* SCRIPT_H_ */
