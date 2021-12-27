
TARGET=nelgscript
DATA_PREFIX=./

# I don't know how you're "supposed" to do this
# however with this one should be able to do CFLAGS=-DWHATEVER make
CFLAGS_=-O2 $(CFLAGS)


# check PREFIX
ifeq ($(PREFIX),)
	PREFIX=/usr/local/
endif

ifeq ($(DATA_PREFIX),)
	DATA_PREFIX=$(PREFIX)/share/
endif


OBJECTS=\
obj/main.o \
obj/core.o \
obj/error.o \
obj/Variable.o \
obj/variables.o \
obj/functions.o \
obj/parser.o \


install_bin = install -g $(USER) -o $(USER) -m 755


all: $(TARGET)


run: all
	./$(TARGET)

# rdynamic is needed for ERR_C_BT to show the function names
# so when an error occurs, it will sort-of show a backtrace
# based on the nelgscript code
debug:
	@CFLAGS="$(CFLAGS) -O0 -g -rdynamic -DERR_C_BT -Wall" $(MAKE)

run-debug: debug
	gdb -ex=run -ex=bt -ex=quit --args ./$(TARGET) $(ARGS)

debug-run: run-debug


clean:
	-rm -f $(TARGET) $(TARGET).exe
	-rm -f obj/*.o

clean-dist:
	-rm -f obj/*.o
	-rmdir obj/


install: $(TARGET)
	mkdir -p $(PREFIX)/bin/
	-$(install_bin) $(TARGET) $(PREFIX)/bin/$(TARGET)
	

uninstall:
	rm    $(PREFIX)/bin/$(TARGET)


emscripten:
	@emmake $(MAKE) PLATFORM=emscripten


# executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS_) $(OBJECTS) -o $(TARGET) -lm 

obj:
	@mkdir -p obj

# objects
obj/main.o: src/nelgscript.c | obj
	$(CC) $(CFLAGS_) src/nelgscript.c -c -o obj/main.o

obj/core.o: src/core.c src/core.h
	$(CC) $(CFLAGS_) src/core.c -c -o obj/core.o

obj/error.o: src/error.c src/error.h
	$(CC) $(CFLAGS_) src/error.c -c -o obj/error.o

obj/Variable.o: src/variables/Variable.c src/variables/Variable.h
	$(CC) $(CFLAGS_) src/variables/Variable.c -c -o obj/Variable.o

obj/variables.o: src/variables/variables.c src/variables/variables.h
	$(CC) $(CFLAGS_) src/variables/variables.c -c -o obj/variables.o

obj/functions.o: src/functions/functions.c src/functions/functions.h
	$(CC) $(CFLAGS_) src/functions/functions.c -c -o obj/functions.o

obj/parser.o: src/parser.c src/parser.h
	$(CC) $(CFLAGS_) src/parser.c -c -o obj/parser.o


