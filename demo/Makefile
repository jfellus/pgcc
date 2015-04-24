all: pgcc compile

pgcc: 
	cd ..; ./pgcc demo/basic_vision.script demo/basic_vision
	
compile: pgcc
	cd basic_vision; make

.PHONY: pg