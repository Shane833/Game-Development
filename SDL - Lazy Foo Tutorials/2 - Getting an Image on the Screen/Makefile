CFLAGS=-Wall -g -O2 -Isrc/headers
LIBS=-lmingw32 -lSDL2main -lSDL2
LIBSL=-ldl -lSDL2main -lSDL2

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TARGET=main

all: compile link

debug: compile
	$(CC) $(OBJECTS) -DNDEBUG -o $(TARGET) $(LIBS)

compile: $(SOURCES) $(OBJECTS)

link:
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBSL)

clean:
	del /q src\*.o
	del /q $(TARGET).exe