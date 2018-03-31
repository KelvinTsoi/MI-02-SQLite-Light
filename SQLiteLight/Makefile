SHELL = /bin/bash

AllDirs     := $(shell ls -R | grep '^\./.*:$$' | awk '{gsub(":","");print}') .
C_Sources   := $(foreach n,$(AllDirs),$(wildcard $(n)/*.c))
CPP_Sources := $(foreach n,$(AllDirs),$(wildcard $(n)/*.cpp))

Sources = $(C_Sources) $(CPP_Sources)

OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(Sources)))
Depends := $(patsubst %.c,%.d,$(patsubst %.cpp,%.d,$(Sources)))

TARGET  := SQLiteLight 

CC      := gcc
CXX     := g++

LIBS    := -ldl -lstdc++ -lpthread
DEFINES := -fPIC -Wall
INCLUDE := $(foreach n,$(AllDirs) , -I$(n))
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS)


.PHONY : everything objs clean rebuild

everything : $(TARGET)

all : $(TARGET)

objs : $(OBJS)

rebuild: everything

clean :
	rm -rf $(OBJS) $(TARGET) $(Depends)

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)
