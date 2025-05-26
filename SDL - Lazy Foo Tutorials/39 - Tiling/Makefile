CFLAGS=-Wall -O2 -Isrc/headers -fsanitize=address
LIBS_STATIC=-static-libgcc -static-libstdc++ -lavif -laom -ldav1d -lrav1e -lsharpyuv -lsvtav1enc -lyuv -ljpeg -ljxl -lpng -ltiff -lwebp -lwebpdemux -lfreetype -lharfbuzz -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 
LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2
LIBSL = -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2
SANITIZE = -fsanitize=address
STATIC=-static

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

static: CFLAGS=-Wall -O2 -Isrc/headers
static: compile links

compile: $(SOURCES) $(OBJECTS)

link:
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

links:
	$(CC) $(STATIC) $(OBJECTS) -o $(TARGET) $(LIBS_STATIC)
	
linkl:
	$(CC) $(SANITIZE) $(OBJECTS) -o $(TARGET) $(LIBSL)

cleanw:
	del /q src\*.o
	del $(TARGET).exe

cleanl:
	rm -f src/*.o
	rm -f $(TARGET)