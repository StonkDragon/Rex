runtime:
		gcc -Wunsequenced -o bin/rex rex.cpp

runtime-debug:
		gcc -Wunsequenced -o bin/rex-debug rex.cpp -DDEBUG

compiler:
		gcc -Wunsequenced -o bin/rasm rasm.cpp

compiler-debug:
		gcc -Wunsequenced -o bin/rasm-debug rasm.cpp -DDEBUG

all:
		gcc -Wunsequenced -o bin/rasm rasm.cpp
		gcc -Wunsequenced -o bin/rex rex.cpp

run:
		./bin/rex test.rasm.out
