runtime:
		gcc -o bin/rex src/rex.cpp

runtime-debug:
		gcc -o bin/rex-debug src/rex.cpp -DDEBUG

compiler:
		gcc -o bin/rasm src/rasm.cpp

compiler-debug:
		gcc -o bin/rasm-debug src/rasm.cpp -DDEBUG

all:
		gcc -o bin/rasm src/rasm.cpp
		gcc -o bin/rex src/rex.cpp

all-debug:
		gcc -o bin/rex-debug src/rex.cpp -DDEBUG
		gcc -o bin/rasm-debug src/rasm.cpp -DDEBUG

hello-example:
		gcc -o bin/rex-debug src/rex.cpp -DDEBUG
		gcc -o bin/rasm-debug src/rasm.cpp -DDEBUG
		clear

		./bin/rasm-debug tests/hello.rasm
		./bin/rex-debug tests/hello.rx
