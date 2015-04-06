/*
 * pgcc.h
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#ifndef PGCC_H_
#define PGCC_H_


#include "model/model.h"
#include "io/writer.h"
#include "io/reader.h"


void compile_script(const std::string& filename, const std::string& out_dir);


#endif /* PGCC_H_ */
