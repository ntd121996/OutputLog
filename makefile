CUR_DIR =
CC = gcc
CXX = g++
LIBS =
CXXFLAGS = -g -Wall
CXXFLAGS += -std=c++11
DEFINE = -DENABLE_DEBUG
#Include Direction
INCLUDE_DIR = include

#Source File Direction
SOURCEC_DIR = source

INCLUDE_BUILD = -I$(INCLUDE_DIR) \
-I.

DEBUG = Debug# Create folder to save objects
RELEASE = Release# Create folder to save objects

#Source File .cpp
SOURCE = \
main.cpp \
Log.cpp \


#Source fullpath to build
SOURCE_BUILD_DEBUG = $(patsubst %.cpp,$(DEBUG)/%.o,$(SOURCE))#Convert .cpp -> .o with new folder
SOURCE_BUILD_RELEASE = $(patsubst %.cpp,$(RELEASE)/%.o,$(SOURCE))#Convert .cpp -> .o with new folder
# patsubst( pattern ,replacement, text)

EXE = main
REMOVE = rm -rf


#Suffix Rule
$(DEBUG)/%.o : $(SOURCEC_DIR)/%.cpp  #compile with new folder
	$(CXX) -c $(CXXFLAGS) $(DEFINE) -o $@ $< $(INCLUDE_BUILD)

$(RELEASE)/%.o : $(SOURCEC_DIR)/%.cpp  #compile with new folder
	$(CXX) -c $(CXXFLAGS) -o $@ $< $(INCLUDE_BUILD)

.PHONY: all
all: debug release

.PHONY: debug
debug: debugdir $(DEBUG)/$(EXE)

.PHONY: release
release: releasedir $(RELEASE)/$(EXE)

$(DEBUG)/$(EXE): $(SOURCE_BUILD_DEBUG)#linker with new folder
	@echo Linking ...
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE_BUILD)$(LIBS)
	@echo Build Success !!!

$(RELEASE)/$(EXE): $(SOURCE_BUILD_RELEASE)#linker with new folder
	@echo Linking ...
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE_BUILD)$(LIBS)
	@echo Build Success !!!

test:
	@echo $(SOURCE_BUILD)
	@echo $(DEPS_BUILD)

rundebug:
	./$(DEBUG)/$(EXE)

runrelease:
	./$(RELEASE)/$(EXE)

.PHONY: clean
clean :
	$(REMOVE) $(DEBUG)/*.o $(EXE)
	$(REMOVE) $(RELEASE)/*.o $(EXE)
	@echo Remove Success

.PHONY: debugdir
debugdir:
	[ -d $(DEBUG) ] || mkdir -p $(DEBUG)

.PHONY: releasedir
releasedir:
	[ -d $(RELEASE) ] || mkdir -p $(RELEASE)