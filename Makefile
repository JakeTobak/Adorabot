CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=src/main.cpp src/SweetieBot.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sweetiebot

all: $(SOURCES) $(EXECUTABLE)
	mv src/*.o tmp/

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o bin/$@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf tmp/*

remove:
	rm -rf tmp/*.o
	rm -rf bin/$(EXECUTABLE)
