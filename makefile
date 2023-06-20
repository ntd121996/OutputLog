CUR_DIR =
CC = gcc
CXX = g++
LIBS =
CXXFLAGS = -g -Wall
CXXFLAGS += -std=c++11
#Include Direction
INCLUDE_DIR = include

#Source File Direction
SOURCEC_DIR = source

INCLUDE_BUILD = -I$(INCLUDE_DIR) \
-I.

DEBUG = Debug# Create folder to save objects

#Source File .cpp
SOURCE = \
main.cpp \
Log.cpp \


#Source fullpath to build
SOURCE_BUILD = $(patsubst %.cpp,$(DEBUG)/%.o,$(SOURCE))#Convert .cpp -> .o with new folder
# patsubst( pattern ,replacement, text)

EXE = main
REMOVE = rm -rf


#Suffix Rule
$(DEBUG)/%.o : $(SOURCEC_DIR)/%.cpp  #compile with new folder
	$(CXX) -c $(CXXFLAGS) -o $@ $< $(INCLUDE_BUILD)

$(EXE): $(SOURCE_BUILD)#linker with new folder
	@echo Linking ...
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE_BUILD)$(LIBS)
	@echo Build Success !!!

test:
	@echo $(SOURCE_BUILD)
	@echo $(DEPS_BUILD)

run:
	./$(EXE)

.PHONY: clean
clean :
	$(REMOVE) $(DEBUG)/*.o $(EXE)
	@echo Remove Success

.PHONY: makedir
makedir:
	[ -d $(DEBUG) ] || mkdir -p $(DEBUG)