OBJECTS = main.cpp
STD = -std=c++17
MYFLAGS = -Wall -grecord-gcc-switches -Wno-sign-compare -O2 -pipe -Werror=format-security -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-clash-protection

CXX = g++
EXEC = main

all: $(OBJECTS)
	$(CXX) $(STD) $(MYFLAGS) $(OBJECTS) -o $(EXEC)

debug: $(OBJECTS)
	$(CXX) $(STD) -g $(MYFLAGS) $(OBJECTS) -o $(EXEC)

clean: 
	rm $(EXEC)

