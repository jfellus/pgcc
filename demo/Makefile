all: pgcc compile

pgcc: 
	cd ..; ./pgcc demo/ptcg.script demo/ptcg
	
compile: pgcc
	cd ptcg; make

.PHONY: pg