///////////////////////////////////////////////
// PGCC AUTO-GENERATED SCRIPT IMPLEMENTATION //
///////////////////////////////////////////////

/*
	This file was auto-generated by PGCC.
	You shouldn't edit anything in it.
*/


#ifndef __PG_SCRIPT_test_H
#define __PG_SCRIPT_test_H

//////////////
// INCLUDES //
//////////////

#include "../utils/sync_semaphores.h"
#include "/home/jfellus/workspace/pg_nav/src/Hog.h"
#include "/home/jfellus/workspace/pg_nav/src/VLAT.h"
#include "/home/jfellus/workspace/pg_nav/src/Camera.h"
#include "/home/jfellus/workspace/pg_nav/src/Codebook.h"
#include "inner.h"


///////////////////////////////////
// IMPLEMENTATION OF SCRIPT test //
///////////////////////////////////

class Script_test {
public:
	Camera c;
	Hog hog;
	VLAT vlat;
	Codebook dico;
	DumpMatrix dumpvlat;
	Script_inner inner;

	SEM_DECL(inner, inner_out);
	SEM_DECL(inner_in, inner);


	Script_test() {
		c.path = "/dev/video0";
		hog.step = 2;
		dico.D = 128;
		dico.K = 32;
	}

	void init() {
		c.init();
		dico.init();
		inner.init();
		hog.init();
		vlat.init();
		SEM_INIT(inner_in, inner);
		SEM_INIT(inner, inner_out);
		dumpvlat.init();
	}


/////////////////
// THREADS (3) //
/////////////////

	void process_thread_0() {
		c.process();
		dico.process();
		hog.process(c);
		vlat.process(hog, dico);
		inner.in.process(vlat);
		SEM_SEND(inner_in, inner);
	}

	void process_thread_1() {
		SEM_RECV(inner, inner_out);
		dumpvlat.process(inner.out);
	}

	void process_thread_2() {
		SEM_RECV(inner_in, inner);
		inner.process_thread_0();
		SEM_SEND(inner, inner_out);
	}

};

#endif // __PG_SCRIPT_test_H
