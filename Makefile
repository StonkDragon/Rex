CC=clang
CFLAGS=-Wall -Wextra -std=gnu18 -Wgnu-binary-literal -pedantic -ggdb -O2

all:
		mkdir -p bin
		$(CC) $(CFLAGS) -o bin/rasm src/rasm.c
		$(CC) $(CFLAGS) -o bin/rex src/rex.c

runtime:
		$(CC) $(CFLAGS) -o bin/rex src/rex.c

runtime-debug:
		$(CC) $(CFLAGS) -o bin/rex-debug src/rex.c -DDEBUG

compiler:
		$(CC) $(CFLAGS) -o bin/rasm src/rasm.c

compiler-debug:
		$(CC) $(CFLAGS) -o bin/rasm-debug src/rasm.c -DDEBUG

all-debug:
		$(CC) $(CFLAGS) -o bin/rex-debug src/rex.c -DDEBUG
		$(CC) $(CFLAGS) -o bin/rasm-debug src/rasm.c -DDEBUG

hello-example:
		$(CC) $(CFLAGS) -o bin/rex-debug src/rex.c -DDEBUG
		$(CC) $(CFLAGS) -o bin/rasm-debug src/rasm.c -DDEBUG
		clear

		./bin/rasm-debug tests/hello.rasm
		./bin/rex-debug tests/hello.rx
