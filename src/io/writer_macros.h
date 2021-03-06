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
#define FUNCTION(ret,name,params) do {f << #ret << " " << TOSTRING(name) << #params << " {\n"; NESTED_LEVEL++; } while(0)
#define I(x)                      f << "\t" << #x << "\n";
#define J(x)                      f << "\t" << TOSTRING(x) << "\n";
#define II(x)                      f << REPEAT_STR("\t", NESTED_LEVEL) << #x << "\n"
#define JJ(x)                      f << REPEAT_STR("\t", NESTED_LEVEL) << TOSTRING(x) << "\n"
#define END_FUNCTION() do {f << "}\n\n"; NESTED_LEVEL--;} while(0)
#define CLASS(c)		do {f << "class " << TOSTRING(c) << " {\n" << "public:\n"; NESTED_LEVEL++; } while(0)
#define END_CLASS() do{f << "};\n\n"; NESTED_LEVEL--; } while(0)
#define METHOD(ret,x,params) do{f << "\t" << #ret << " " << TOSTRING(x) << #params << " {\n";NESTED_LEVEL++; } while(0)
#define END_METHOD()  do{f << "\t}\n\n";NESTED_LEVEL--; } while(0)
#define CONSTRUCTOR(x,params) do{f << "\t" << TOSTRING(x) << #params << " {\n";NESTED_LEVEL++; } while(0)
#define END_CONSTRUCTOR() do{f << "\t}\n\n";NESTED_LEVEL--; } while(0)



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
