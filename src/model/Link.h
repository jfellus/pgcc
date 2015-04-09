/*
 * Link.h
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#ifndef LINK_H_
#define LINK_H_


class Link {
public:
	Module* src;
	Module* dst;
	std::string src_pin;
	std::string dst_pin;
	std::string type;
	bool bNoData;

	Link(Module* src, const std::string& src_pin, Module* dst, const std::string& dst_pin, const std::string& type);
	~Link();

	inline bool is_async() {return type=="/";}
	inline bool is_sync() {return type=="-";}
	inline bool is_same_thread() {return type=="" || is_virtual();}
	inline bool is_virtual() {return type=="V"; }

	inline bool has_data() {return !is_virtual() && !bNoData;}

	inline bool is_timescale_link() {
		return src->is_timescale() || dst->is_timescale();
	}
};


#endif /* LINK_H_ */
