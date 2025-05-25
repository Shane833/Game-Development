CFLAGS=-Wall -O2 -Isrc/headers -fsanitize=address
LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 
LIBSL = -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2
SANITIZE = -fsanitize=address

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TARGET=main

# Can't figure out how to make ASAN work on windows
# Turns out GCC on windows(msys2) don't have ASAN support
# Howerver CLANG has, but I don't think I will be working with clang just yet
windows: CFLAGS=-Wall -O2 -Isrc/headers
windows: compile link

linux: compile linkl

debug: CFLAGS += -DNDEBUG
debug: compile link

compile: $(SOURCES) $(OBJECTS)

link:
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)
	
linkl:
	$(CC) $(SANITIZE) $(OBJECTS) -o $(TARGET) $(LIBSL)

cleanw:
	del /q src\*.o
	del $(TARGET).exe

cleanl:
	rm -f src/*.o
	rm -f $(TARGET)