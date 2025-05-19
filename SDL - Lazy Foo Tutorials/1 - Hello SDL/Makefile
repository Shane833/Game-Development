CFLAGS=-Wall -g -O2 -Isrc/headers
LIBS=-lmingw32 -lSDL2main -lSDL2
LIBSL=-lSDL2main -lSDL2

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TARGET=main

all: compile link

compile: $(SOURCES) $(OBJECTS)

link:
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBSL)

clean:
	del /q src\*.o

cleanl:
	rm -f src/*.o
	rm -f $(TARGET)