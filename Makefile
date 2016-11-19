###                                                                          ###
#  teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph    #
#                                                                              #
#  This Makefile is based on https://github.com/jkvis/generic-cpp-template     #
###                                                                          ###

CXX          = g++
LDFLAGS      = -Iinclude
CXXFLAGS     = -pedantic -Wall -march=native -fopenmp 
LISTENERFLAG = -Drunlistener=1
DEBUGFLAGS   = -O0 -ggdb3 -Wextra 
RELEASEFLAGS = -O3 -DNDEBUG #-fwhole-program
PROFILEFLAGS = $(RELEASFLAGS) -pg

TARGET       = teexgraph
SOURCEDIR    = src/
SOURCES      = $(shell find $(SOURCEDIR) -name '*.cpp')
INCLUDEDIR   = src/
HEADERS      = $(shell find $(INCLUDEDIR) -name '*.h')
COMMON       = 
OBJECTS      = $(SOURCES:.cpp=.o)

PREFIX       = $(DESTDIR)/usr/local
BINDIR       = $(PREFIX)/bin

all: release

listener: CXXFLAGS += $(LISTENERFLAG)
listener: $(TARGET)

release: CXXFLAGS += $(RELEASEFLAGS)
release: $(TARGET)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

profile: CXXFLAGS += $(PROFILEFLAGS)
profile: $(TARGET)

install: release
	install -D $(TARGET) $(BINDIR)/$(TARGET)

install-strip: release
	install -D -s $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm $(BINDIR)/$(TARGET)

clean:
	rm -f $(OBJECTS) gmon.out $(TARGET)

distclean: clean
	rm -f $(TARGET)

rebuild: clean all

tarball:
	tar -cvzf $(TARGET).tar.gz $(HEADERS) $(SOURCES) $(COMMON) Makefile

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ -c $<

.PHONY: all release debug profile install install-strip uninstall clean distclean rebuild tarball
