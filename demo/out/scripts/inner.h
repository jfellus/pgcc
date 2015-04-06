///////////////////////////////////////////////
// PGCC AUTO-GENERATED SCRIPT IMPLEMENTATION //
///////////////////////////////////////////////

/*
	This file was auto-generated by PGCC.
	You shouldn't edit anything in it.
*/


#ifndef __PG_SCRIPT_inner_H
#define __PG_SCRIPT_inner_H

//////////////
// INCLUDES //
//////////////

#include "../utils/sync_semaphores.h"
#include "/home/jfellus/workspace/pg_nav/src/Hog.h"
#include "/home/jfellus/workspace/pg_nav/src/VLAT.h"
#include "/home/jfellus/workspace/pg_nav/src/Camera.h"
#include "/home/jfellus/workspace/pg_nav/src/Codebook.h"


////////////////////////////////////
// IMPLEMENTATION OF SCRIPT inner //
////////////////////////////////////

class Script_inner {
public:
	Camera c;
	Matrix m;
	In<Matrix> in;
	Out<Image> out;



	Script_inner() {
		c.path = "/dev/video0";
	}

	void init() {
		c.init();
		in.init();
		m.init();
		out.init();
	}


/////////////////
// THREADS (1) //
/////////////////

	void process_thread_0() {
		c.process();
		m.process(in, c);
		out.process(m);
	}

};

#endif // __PG_SCRIPT_inner_H
