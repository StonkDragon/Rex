CC=clang
CFLAGS=-Wall -Wextra -std=gnu18 -Wgnu-binary-literal -pedantic -ggdb -O2

compile:
		mkdir -p bin
		$(CC) $(CFLAGS) -o bin/rex src/rex.c

compile-debug:
		mkdir -p bin
		$(CC) $(CFLAGS) -o bin/rex-debug src/rex.c -DDEBUG

hello-example:
		mkdir -p bin
		$(CC) $(CFLAGS) -o bin/rex-debug src/rex.c -DDEBUG

		./bin/rex-debug comp tests/hello.rasm
		./bin/rex-debug run tests/hello.rasm
