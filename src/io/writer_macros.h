/*
 * writer_macros.h
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#ifndef HEADER



#define HEADER()          write_header(f)
#define INCLUDE_SYS(path)     f << "#include <" << path << ">\n";
#define INCLUDE(path)     f << "#include \"" << path << "\"\n";
#define ________          f << "\n";
#define COMMENT(x)        write_cpp_comment_box(f, TOSTRING(x)); f << "\n";
#define FUNCTION(ret,name,params) f << #ret << " " << TOSTRING(name) << #params << " {\n";
#define I(x)                      f << "\t" << #x << "\n";
#define J(x)                      f << "\t" << TOSTRING(x) << "\n";
#define II(x)                      f << "\t\t" << #x << "\n";
#define JJ(x)                      f << "\t\t" << TOSTRING(x) << "\n";
#define END_FUNCTION() f << "}\n\n";
#define CLASS(c)		f << "class " << TOSTRING(c) << " {\n" << "public:\n";
#define END_CLASS() f << "};\n\n";
#define METHOD(ret,x,params) f << "\t" << #ret << " " << TOSTRING(x) << #params << " {\n";
#define END_METHOD() f << "\t}\n\n";
#define CONSTRUCTOR(x,params) f << "\t" << TOSTRING(x) << #params << " {\n";
#define END_CONSTRUCTOR() f << "\t}\n\n";



#else



#undef HEADER
#undef INCLUDE_SYS
#undef INCLUDE
#undef ________
#undef COMMENT
#undef FUNCTION
#undef I
#undef J
#undef II
#undef JJ
#undef END_FUNCTION
#undef CLASS
#undef END_CLASS
#undef METHOD
#undef END_METHOD
#undef CONSTRUCTOR
#undef END_CONSTRUCTOR



#endif
