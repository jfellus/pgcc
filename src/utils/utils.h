/*
 * utils.h
 *
 *  Created on: 4 avr. 2015
 *      Author: jfellus
 */

#ifndef PG_UTILS_H_
#define PG_UTILS_H_

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "string.h"
#include <stdexcept>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>


template <typename T> class array {
public:
	std::vector<T> v;
public:
	array() {}

	inline const T& add(const T& e) {v.push_back(e); return e;}

	inline bool remove(const T& e) {
		typename std::vector<T>::iterator position = std::find(v.begin(), v.end(), e);
		if (position != v.end()) {v.erase(position); return true;}
		return false;
	}

	inline size_t size() {return v.size();}

	inline T& operator[](int i) {return v[i];}
};



inline void copy_file_to(const std::string& filename, std::ofstream& f) {
	std::ifstream i(filename.c_str());
	std::string line;
	while (std::getline(i, line)) {
		f << line << "\n";
	}
	i.close();
}

inline void write_cpp_comment_box(std::ofstream& f, const std::string& s) {
	for(uint i=0; i<s.length()+(2+2+1+1); i++) f << "/"; f<<"\n";
	f << "// " << s << " //\n";
	for(uint i=0; i<s.length()+(2+2+1+1); i++) f << "/"; f<<"\n";
}

#define DBG(x) std::cout << x << "\n";
#define ERROR(x) do { std::cout << "ERROR : " << x << "\n"; throw std::runtime_error(TOSTRING(x)); } while(0)

#define SYSTEM(x) (void) system(TOSTRING(x).c_str())

inline void mkdir(const std::string& s) { SYSTEM("mkdir -p " << s); }
inline void mkrmdir(const std::string& s) { SYSTEM("rm -rf " << s); mkdir(s); }


#define MIN(a,b) ((a) > (b) ? (a) : (b))



inline bool file_exists(const std::string& filename) {
	  struct stat   buffer;
	  return (stat (filename.c_str(), &buffer) == 0);
}

inline bool file_is_directory(const std::string& filename) {
	struct stat   buffer;
	if(stat (filename.c_str(), &buffer) != 0) return false;
    return (S_ISDIR (buffer.st_mode));
}

#define SYSTEM_OUTPUT(x) system_output(TOSTRING(x))
inline std::string system_output(const std::string& cmd) {
	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe) return "ERROR";
	char buffer[262144];
	std::string data;
	std::string result;
	int dist=0;
	int size;
	while(!feof(pipe)) {
		size=(int)fread(buffer,1,262144, pipe);
		data.resize(data.size()+size);
		memcpy(&data[dist],buffer,size);
		dist+=size;
	}
	pclose(pipe);
	return data;
}


#endif /* UTILS_H_ */
