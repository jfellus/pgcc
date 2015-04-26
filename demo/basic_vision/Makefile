LD_FLAGS:= -L/home/jfellus/workspace/pg_vision -L/home/jfellus/workspace/pg_vision -Wl,-rpath,/home/jfellus/workspace/pg_vision -lpg_vision -ljpeg -L/home/jfellus/workspace/pg_basics -L/home/jfellus/workspace/pg_gtk -L/home/jfellus/workspace/pg_gtk/. -Wl,-rpath,/home/jfellus/workspace/pg_gtk/. -lpg_gtk `pkg-config --libs gtk+-3.0` -L/home/jfellus/workspace/pg_learning -L/home/jfellus/workspace/pg_learning -Wl,-rpath,/home/jfellus/workspace/pg_learning -lpg_learning -fopenmp -L/home/jfellus/workspace/pg_algebra -L/home/jfellus/workspace/pg_algebra -Wl,-rpath,/home/jfellus/workspace/pg_algebra -lpg_algebra -lm
CFLAGS:= `pkg-config --cflags gtk+-3.0` -fopenmp
CFLAGS=


##################### don't edit below this line unless you're a GNU make's expert ! ##########

all: main

main: main.cpp scripts/*.h utils/*.h
	g++ -g -O3 -o main main.cpp -lpthread -I$(HOME)/.pgcc/includes $(CFLAGS) $(LD_FLAGS)
	
clean: 
	@rm -rf main
	
export: main
	@rm -f main.tar.gz
	tar -pczf main.tar.gz .
	
.PHONY: clean export
