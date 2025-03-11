CFLAGS=-Wall -g -Isrc/headers -O2
LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2
LIBSL=-lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2


SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TARGET=main

all: compile link

debug: CFLAGS += -DNDEBUG
debug: all

linux: compile linkl

compile: $(SOURCES) $(OBJECTS)

link:
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)
	
linkl:
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBSL)
	
clean:
	del /q $(TARGET).exe
	del /q src\*.o

cleanl:
	rm -f $(TARGET)
	rm -f src/*.o