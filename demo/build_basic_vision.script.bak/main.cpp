///////////////////////////////////////////////
// PGCC AUTO-GENERATED SCRIPT IMPLEMENTATION //
///////////////////////////////////////////////

/*
	This file was auto-generated by PGCC.
	You shouldn't edit anything in it.
*/



#include <pthread.h>
#include <unistd.h>
#include <pg.h>

#include "scripts/basic_visionee.h"


// The main script 
Script_basic_visionee main_script;


/////////////
// THREADS //
/////////////

void* f_thread_0(void*) { try { for(;;) main_script.process_thread_0(); } catch(std::runtime_error& x) { PRINT_FATAL_ERROR(x.what()); } return 0;}


//////////
// MAIN //
//////////

int main(void) {
	main_script.init();
	pthread_t th0; pthread_create(&th0, 0, f_thread_0, 0);
	pthread_join(th0,0);
}

