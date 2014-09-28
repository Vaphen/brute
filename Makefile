#---------------------------------------------------
# Makefile for Project brute
# Mail: Igel101@web.de
# Last edited: 28.09.2014 
#---------------------------------------------------

PROGRAM_NAME = brute
SOURCES = main.cpp brute.cpp brute.hpp http_req.cpp http_req.hpp param.cpp param.hpp m_threads.cpp m_threads.hpp
PARAMS = -pthread -std=c++0x -g -Wall -lcurl


all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(SOURCES)
	g++ $(SOURCES) $(PARAMS) -o $(PROGRAM_NAME)

clean:
	rm -f $(PROGRAM_NAME)

