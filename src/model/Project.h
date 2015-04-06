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

public:
	Project(const std::string& filename) : filename(filename) {
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
			else if(str_starts_with(line, "Library")) libraries.add(resolve(str_trim(str_after(line, "Library"))));
			else if(str_starts_with(line, "IncludePath")) include_paths.add(resolve(str_trim(str_after(line, "IncludePath"))));
		}
	}

	std::string resolve(const std::string& path) {
		if(str_starts_with(path, "/")) return path;
		return TOSTRING(str_dirname(filename) << "/" << path);
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
