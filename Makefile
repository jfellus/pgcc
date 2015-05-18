
###### INSTALLATION GUIDE ######
# make external_libs
# make
# ./coeos++
################################


APT_GET_DEPENDENCIES:= 


REQUIRED_PACKAGES:= 
REQUIRED_LIBS:= -lpthread 


INCLUDE_PATHS:=

SRC_DIR:=./src

EXECUTABLE:=pgcc







########################## DON'T EDIT BELOW THIS LINE (unless you are a gnu make's expert ##############

SRC := $(shell find $(SRC_DIR) -name '*.cpp') 
OBJS := $(addprefix bin/,$(SRC:.cpp=.o))

all: $(EXECUTABLE) 
$(EXECUTABLE): $(OBJS)

CXXFLAGS := -fPIC -g -rdynamic -Wall -MMD $(addprefix -I,$(INCLUDE_PATHS))
LDFLAGS := -fPIC -rdynamic 
DEPENDS = $(OBJS:.o=.d)    


install:
	@ln -fs `pwd`/resources/pgcc.sh /usr/local/bin/pgcc 
	@ln -fs `pwd`/resources/pgcc_add_project.sh /usr/local/bin/pgcc_add_project 
	@ln -fs `pwd`/resources/pgcc_resolve_project.sh /usr/local/bin/pgcc_resolve_project 
	@echo "Installation successful"
	
	
$(EXECUTABLE) : $(OBJS)          
	@echo "Build executable $@"
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS) 
	


bin/%.o: %.cpp
	@mkdir -p `dirname $(@:.o=.d)`
	@touch $(@:.o=.d)
	@echo "Compilation : $< "
	@g++ $(CXXFLAGS) -MMD -c $< -o $@

bin: 
	@mkdir -p bin

clean:
	@rm -f $(EXECUTABLE)
	@rm -rf bin
	


external_libs:
	@echo "We need your password for this : "
	@sudo echo "thank you"
	@sudo apt-get install $(APT_GET_DEPENDENCIES)
	@echo "DONE"
	

-include $(DEPENDS) 
