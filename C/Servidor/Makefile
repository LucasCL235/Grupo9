CC=gcc
STANDARD=c99
FLAGS=-lpthread
CFLAGS=-c $(FLAGS)
LDFLAGS=$(FLAGS)
STDFLAG=-std=$(STANDARD)
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=servidor

all: $(SOURCES) $(EXECUTABLE)
	
run: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(STDFLAG)

debug: $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS) $(STDFLAG) -g -DDEBUG

test: $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS) $(STDFLAG) -g -DDEBUG -DTEST
	./$(EXECUTABLE)

sublimegdb: $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS) $(STDFLAG) -g -DDEBUG -DSGDB -DTEST

.c.o:
	$(CC) $< -o $@ $(CFLAGS) $(STDFLAG)

clear:
	rm *.o &
	rm $(EXECUTABLE)