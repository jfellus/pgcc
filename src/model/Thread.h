/*
 * Thread
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#ifndef THREAD_
#define THREAD_



class Thread {
public:
	Script* script;
	uint id;
	array<Module*> modules;
	array<Module*> root_modules;
	bool isConsumed;
	uint nbProcessables;

public:
	Thread(Script* script, uint id);
	~Thread();

	void add(Module* m);

	void consume() {isConsumed = true;}
	void reset_consume() {isConsumed = false;}
};




#endif /* THREAD_ */
