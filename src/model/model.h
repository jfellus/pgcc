/*
 * model.h
 *
 *  Created on: 4 avr. 2015
 *      Author: jfellus
 */

#ifndef PG_MODEL_H_
#define PG_MODEL_H_


#include "../utils/utils.h"

class Module;
class Link;
class Script;
class Project;
class Thread;


Script* read_script(const std::string& filename);

#include "Project.h"
#include "Thread.h"
#include "Module.h"
#include "Link.h"
#include "Script.h"











#endif /* PG_MODEL_H_ */
