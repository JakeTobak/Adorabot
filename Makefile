CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=src/main.cpp src/Adorabot.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Adorabot

all: $(SOURCES) $(EXECUTABLE)
	mv src/*.o tmp/

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o bin/$@ -lcurl

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ -lcurl

clean:
	rm -rf tmp/*.o

remove:
	rm -rf tmp/*.o
	rm -rf bin/$(EXECUTABLE)
