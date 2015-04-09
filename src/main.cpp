/*
 * main.cpp
 *
 *  Created on: 4 avr. 2015
 *      Author: jfellus
 */

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <stdint.h>
#include <algorithm>
#include <list>

#include "pgcc.h"


void usage() {
	DBG("pgcc - Promethe-to-GCC Neural Nets compiler");
	DBG("Usage : pgcc <main_script_file> <output_directory>");
	exit(1);
}


int main(int argc, char **argv) {
	if(argc<3) usage();
	try {
	compile_script(argv[1],argv[2]);
	} catch(std::runtime_error&) {exit(1);}
}
