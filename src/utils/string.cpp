/*
 * string.cpp
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#include "utils.h"
#include <algorithm>
#include <libgen.h>

std::string str_replace(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

bool str_ends_with(const std::string& s, const std::string& suffix) {
	if(s.length() < suffix.length()) return false;
	return s.compare (s.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool str_starts_with(const std::string& s, const std::string& prefix) {
	return s.compare(0, prefix.length(), prefix)==0;
}

std::string str_after(const std::string& s, const std::string& prefix) {
	uint i = s.find(prefix);
	if(i==std::string::npos) return s;
	return s.substr(i+prefix.length());
}

std::string str_before(const std::string& s, const std::string& suffix) {
	uint i = s.find(suffix);
	if(i==std::string::npos) return s;
	return s.substr(0,i);
}

std::string str_between(const std::string& s, const std::string& prefix, const std::string& suffix) {
	return str_before(str_after(s,prefix), suffix);
}

bool str_has(const std::string& s, const std::string& needle) {
	return s.find(needle)!=std::string::npos;
}

std::string str_to_lower(const std::string& s) {
	std::string a = s;
	std::transform(a.begin(), a.end(), a.begin(), ::tolower);
	return a;
}


std::string url_decode(const std::string& s) {
	return str_replace(str_replace(s, "%0A", "\n"), "%20", " ");
}

std::string JSON_escape(const std::string& s) {
	return str_replace(str_replace(s, "\"", "\\\""), "\n", "\\n");
}

std::string str_trim(const std::string& s) {
	int i=0;
	while(isspace(s[i])) i++;
	int j=s.length()-1;
	while(isspace(s[j])) j--;
	return s.substr(i,j-i+1);
}


void str_remove(std::string& s, const std::string& what) {
	size_t i = s.find(what);
	if(i!=std::string::npos) s.erase(s.find(what), what.length());
}

std::string str_nth_occurence(const std::string& s, const std::string& needle, unsigned int N) {
	int z = needle.size();
	size_t pos=0,from=0;
	unsigned i=0;
	while ( i<N ) {
		pos=s.find(needle,from);
		if ( std::string::npos == pos ) { return ""; }
		from = pos + z;
		++i;
	}
	size_t j = s.find(needle, from);
	if(pos>0) pos+=z;
	if(std::string::npos == j) return s.substr(pos);
	return s.substr(pos, j-pos);
}

std::string str_param(const std::string& s, int iParam) {
	return str_nth_occurence(s, " ", iParam);
}

std::string str_align(const std::string& s, int nbchars) {
	std::ostringstream str; str << s;
	for(int i=s.length(); i<=nbchars; i++) str << " ";
	return str.str();
}

std::string str_dirname(const std::string& path) {	char s[MAX_PATH];strcpy(s,path.c_str());return dirname((char*)s);}
std::string str_basename(const std::string& path) {	char s[MAX_PATH];strcpy(s,path.c_str());return basename((char*)s);}

std::string str_repeat(std::string s, int nb) {
	std::ostringstream ss;
	for(;nb>0; nb--) ss << s;
	return ss.str();
}

