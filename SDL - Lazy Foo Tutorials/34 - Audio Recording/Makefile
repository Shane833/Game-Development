CFLAGS=-Wall -g -O2 -Isrc
LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2
LIBSL = -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2
SANITIZE = -fsanitize=address
SANITIZEW = -fsanitize=undefined -fsanitize-trap

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TARGET=main

# Can't figure out how to make ASAN work on windows
# Turns out GCC on windows(msys2) don't have ASAN support
# Howerver CLANG has, but I don't think I will be working with clang just yet
windows: CFLAGS += $(SANITIZEW)
windows: compile link

linux: CFLAGS += $(SANITIZE)
linux: compile linkl

debug: CFLAGS += -DNDEBUG
debug: compile link

compile: $(SOURCES) $(OBJECTS)

link:
	$(CC) $(SANITIZEW) $(OBJECTS) -o $(TARGET) $(LIBS)

	
linkl:
	$(CC) $(SANITIZE) $(OBJECTS) -o $(TARGET) $(LIBSL)

cleanw:
	del /q src\*.o
	del $(TARGET).exe

cleanl:
	rm -f src/*.o
	rm -f $(TARGET)
