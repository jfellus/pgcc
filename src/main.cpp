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
	DBG("Usage : pgcc [-v] <main_script_file> <output_directory>");
	exit(1);
}


int main(int argc, char **argv) {
	int opt;
	while ((opt = getopt(argc, argv, "v")) != -1) {
		switch (opt) {
		case 'v':
			VERBOSE = 1;
			break;
		default: /* '?' */
			usage();
		}
	}

	if(argc-optind<2) usage();
	try {
		compile_script(argv[optind],argv[optind+1]);
	} catch(std::runtime_error&) {exit(1);}
}
