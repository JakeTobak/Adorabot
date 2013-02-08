CC=g++
CFLAGS=-c -Wall -og
LDFLAGS=
SOURCES=src/main.cpp src/Adorabot.cpp src/User.cpp src/Message.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Adorabot

all: $(SOURCES) $(EXECUTABLE)
	mkdir -p tmp/
	mv src/*.o tmp/

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lcurl -lboost_regex -lboost_program_options

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf tmp/*.o

remove:
	rm -rf tmp/
	rm -f $(EXECUTABLE)
