/*
 * Project.h
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#ifndef PROJECT_H_
#define PROJECT_H_



class Project {
public:
	std::string filename;
	array<std::string> includes;
	array<std::string> libraries;
	array<std::string> ld_paths;
	array<std::string> include_paths;
	array<std::string> pkgconfigs;
	array<std::string> cflags;
	array<std::string> ldflags;

public:
	Project(const std::string& filename) : filename(filename) {
		ld_paths.add(str_dirname(filename));
		read(filename);
	}

	void read(const std::string& filename) {
		std::ifstream f(filename.c_str());
		if(!f.good()) ERROR("Can't find project " << filename);
		read(f);
		f.close();
	}

	void read(std::ifstream& f) {
		std::string line;
			while (std::getline(f, line)) {
			line = str_trim(str_before(line, "#"));
			if(line.empty()) continue;
			if(str_starts_with(line, "Include")) includes.add(resolve(str_trim(str_after(line, "Include"))));
			else if(str_starts_with(line, "LDPath")) ld_paths.add(resolve(str_trim(str_after(line, "LDPath"))));
			else if(str_starts_with(line, "Library")) {
				std::string s = str_trim(str_after(line, "Library"));
				if(str_ends_with(s, ".so")) libraries.add(resolve(s));
				else libraries.add(s);
			}
			else if(str_starts_with(line, "IncludePath")) include_paths.add(resolve(str_trim(str_after(line, "IncludePath"))));
			else if(str_starts_with(line, "Pkgconfig")) pkgconfigs.add(str_trim(str_after(line, "Pkgconfig")));
			else if(str_starts_with(line, "CFLAG")) cflags.add(str_trim(str_after(line, "CFLAG")));
			else if(str_starts_with(line, "LDFLAG")) ldflags.add(str_trim(str_after(line, "LDFLAG")));
		}
	}

	std::string resolve(const std::string& path) {
		if(str_starts_with(path, "/")) return path;
		return TOSTRING(str_dirname(filename) << "/" << path);
	}

	std::string get_ld_str() {
		std::ostringstream ss;
		for(uint i=0; i<ld_paths.size(); i++) ss << " -L" << ld_paths[i];
		for(uint i=0; i<libraries.size(); i++) {
			if(str_ends_with(libraries[i], ".so")) {
				std::string path = libraries[i].substr(0,libraries[i].rfind("/"));
				ss << " -L" << path;
				ss << " -Wl,-rpath," << path;
				ss << " -l" << str_between(str_basename(libraries[i]), "lib", ".so");
			}
			else ss << " -l" << libraries[i];
		}
		if(pkgconfigs.size()>0) {
			ss << " `pkg-config --libs";
			for(uint i=0; i<pkgconfigs.size(); i++) ss << " " << pkgconfigs[i];
			ss << "`";
		}
		for(uint i=0; i<ldflags.size(); i++) ss << " " << ldflags[i];
		return ss.str();
	}

	std::string get_cflags_str() {
		std::ostringstream ss;
		if(pkgconfigs.size()>0) {
			ss << " `pkg-config --cflags";
			for(uint i=0; i<pkgconfigs.size(); i++) ss << " " << pkgconfigs[i];
			ss << "`";
		}
		for(uint i=0; i<cflags.size(); i++) ss << " " << cflags[i];
		return ss.str();
	}

};


extern array<Project*> projects;
inline Project* get_project(const std::string& filename) {
	for(uint i=0; i<projects.size(); i++) if(projects[i]->filename==filename) return projects[i];
	Project* p = new Project(filename);
	projects.add(p);
	return p;
}

#endif /* PROJECT_H_ */
