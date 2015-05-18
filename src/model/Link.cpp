/*
 * Link.cpp
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#include "model.h"
#include "Link.h"

Link::Link(Module* src, const std::string& src_pin, Module* dst, const std::string& dst_pin, const std::string& type)
: src(src),dst(dst),src_pin(src_pin),dst_pin(dst_pin),type(type),bNoData(false) {
	if(src) src->outs.add(this);
	if(dst) {
		dst->ins.add(this);
		if(!is_async()) dst->nb_no_async_ins++;
		if(type=="0") bNoData = true;
	}
}

Link::~Link() {
	if(src) src->outs.remove(this);
	if(dst) dst->ins.remove(this);
}
