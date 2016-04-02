CCFLAGS=$(CFLAGS) -std=c++11
LLD_FLAGS=$(LD_FLAGS) -lpgdb `pkg-config --libs libwebsockets`


##################### don't edit below this line unless you're a GNU make's expert ! ##########

all: main debug

main: main.cpp scripts/*.h utils/*.h
	g++ -g -O3 -o main main.cpp -lpthread -I$(HOME)/.pgcc/includes -I/usr/include/pgdb $(CCFLAGS) $(LLD_FLAGS)


debug: main.cpp scripts/*.h utils/*.h
	g++ -DDEBUG -g -O3 -o debug main.cpp -lpthread -I$(HOME)/.pgcc/includes -I/usr/include/pgdb $(CCFLAGS) $(LLD_FLAGS)
	
clean: 
	@rm -rf main
	
export: main
	@rm -f main.tar.gz
	tar -pczf main.tar.gz .
	
.PHONY: clean export