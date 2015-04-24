/*
 * ModulesPrototypes.h
 *
 *  Created on: 10 avr. 2015
 *      Author: jfellus
 */

#ifndef MODULESPROTOTYPES_H_
#define MODULESPROTOTYPES_H_


class ModuleProcessParam {
public:
	std::string cls;
	std::string name;
	ModuleProcessParam(const std::string& cls, const std::string& name) : cls(cls), name(name) {}
};

class ModulePrototype {
	array<ModuleProcessParam> params;
public:
	ModulePrototype() {}

	void add(const std::string& _param) {
		std::string param = str_replace(_param, "\t", " ");
		std::string type = param.substr(0, param.rfind(" "));
		std::string name = param.substr(type.length(), param.length()-type.length());
		params.add(ModuleProcessParam(type,name));
	}

	int get_index(const std::string& param_name) {
		for(uint i=0; i<params.size(); i++) if(params[i].name == param_name) return i;
		return -1;
	}

	const std::string& get_type(int i) {return params[i].cls;	}
	const std::string& get_name(int i) {return params[i].name;	}

	std::string fit(Module* m) {
		uint nbin = 0;
		for(uint i=0; i<m->ins.size(); i++) if(m->ins[i]->has_data()) nbin++;
		if(nbin != params.size()) return "";

		// Single argument : easy !
		if(nbin == 1) {
			std::string val;
			for(uint i=0; i<m->ins.size(); i++) {
				if(!m->ins[i]->has_data()) continue;
				val = m->ins[i]->src->id;
				if(!m->ins[i]->src_pin.empty()) val += "." + m->ins[i]->src_pin;
				return val;
			}
		}

		// Multiple arguments : correctly order with arg names
		std::ostringstream ss;
		for(uint p=0; p<params.size(); p++) {
			if(p!=0) ss << ", ";
			std::string val = "";
			for(uint i=0; i<m->ins.size(); i++) {
				if(!m->ins[i]->has_data()) continue;
				if(m->ins[i]->dst_pin==params[p].name) {
					val = m->ins[i]->src->id;
					if(!m->ins[i]->src_pin.empty()) val += "." + m->ins[i]->src_pin;
				}
			}
			if(val.empty()) ERROR("Missing argument " << params[p].name << " (type=" << params[p].cls << ") for module " << m->id);
			ss << val;
		}

		return ss.str();
	}

};

class ModulesPrototypes {
	std::map<std::string, array<ModulePrototype> > prototypes;
public:
	ModulesPrototypes() {}

	void create_param(const std::string& module_cls, const std::string& param) {
		prototypes[module_cls][prototypes[module_cls].size()-1].add(param);
	}

	void create_process(const std::string& module_cls) {
		prototypes[module_cls].add(ModulePrototype());
	}

	inline std::string compute_params_str(Module* m) {
		for(uint i=0; i<prototypes[m->cls].size(); i++) {
			std::string s = prototypes[m->cls][i].fit(m);
			if(s.empty()) return s;
		}
		return "";
	}

};

#endif /* MODULESPROTOTYPES_H_ */
