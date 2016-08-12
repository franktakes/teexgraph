CC           = g++
FLAGS        = -Iinclude
CFLAGS       = -Wall -Wextra -fopenmp
DEBUGFLAGS   = -O3 -D _DEBUG
RELEASEFLAGS = -O3 -D NDEBUG -fwhole-program -fopenmp

TARGET     = tyxgraph
SOURCEDIR  = ./
SOURCES    = $(shell echo $(SOURCEDIR)*.cpp)
INCLUDEDIR = ./
COMMON     = 
HEADERS    = $(shell echo $(INCLUDEDIR)*.h)
OBJECTS    = $(SOURCES:.cpp=.o)

PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin


all: $(TARGET)

$(TARGET): $(OBJECTS) $(COMMON)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS)

release: $(SOURCES) $(COMMON)
	$(CC) $(FLAGS) $(CFLAGS) $(RELEASEFLAGS) -o $(TARGET) $(SOURCES)

profile: CFLAGS += -pg
profile: $(TARGET)


install: release
	install -D $(TARGET) $(BINDIR)/$(TARGET)

install-strip: release
	install -D -s $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	-rm $(BINDIR)/$(TARGET)


clean:
	-rm -f $(OBJECTS) gmon.out

distclean: clean
	-rm -f $(TARGET)


%.o: %.c $(HEADERS) $(COMMON)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -c -o $@ $<


.PHONY: all profile release install install-strip uninstall clean distclean

