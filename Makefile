CC=g++
CFLAGS=-c -Wextra -og
LDFLAGS=
SOURCES=src/Parser.cpp src/Server.cpp src/Connection.cpp src/User.cpp src/Adorabot.cpp src/Bot.cpp src/MessageFactory.cpp src/main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Adorabot

all: $(SOURCES) $(EXECUTABLE)
	mkdir -p tmp/
	mv src/*.o tmp/

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lcurl -lboost_regex -lboost_program_options -lpthread -lmysqlpp

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf tmp/*.o

remove:
	rm -rf tmp/
	rm -f $(EXECUTABLE)
