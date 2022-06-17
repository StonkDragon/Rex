runtime:
		gcc -Wunsequenced -o bin/rex src/rex.cpp

runtime-debug:
		gcc -Wunsequenced -o bin/rex-debug src/rex.cpp -DDEBUG

compiler:
		gcc -Wunsequenced -o bin/rasm src/rasm.cpp

compiler-debug:
		gcc -Wunsequenced -o bin/rasm-debug src/rasm.cpp -DDEBUG

all:
		gcc -Wunsequenced -o bin/rasm src/rasm.cpp
		gcc -Wunsequenced -o bin/rex src/rex.cpp

all-debug:
		gcc -Wunsequenced -o bin/rex-debug src/rex.cpp -DDEBUG
		gcc -Wunsequenced -o bin/rasm-debug src/rasm.cpp -DDEBUG

hello-example:
		gcc -Wunsequenced -o bin/rex-debug src/rex.cpp -DDEBUG
		gcc -Wunsequenced -o bin/rasm-debug src/rasm.cpp -DDEBUG
		clear

		./bin/rasm-debug tests/hello.rasm
		./bin/rex-debug tests/hello.rx
