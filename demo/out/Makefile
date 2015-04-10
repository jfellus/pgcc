all: main

main: main.cpp scripts/*.h utils/*.h
	g++ -O3 -o main main.cpp -lpthread -I~/.pgcc/includes
	
clean: 
	@rm -rf main
	
export: main
	@rm -f main.tar.gz
	tar -pczf main.tar.gz .
	
.PHONY: clean export