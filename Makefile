runtime:
		gcc -I include -Wunsequenced -o bin/rex src/rex.cpp

runtime-debug:
		gcc -I include -Wunsequenced -o bin/rex-debug src/rex.cpp -DDEBUG

compiler:
		gcc -I include -Wunsequenced -o bin/rasm src/rasm.cpp

compiler-debug:
		gcc -I include -Wunsequenced -o bin/rasm-debug src/rasm.cpp -DDEBUG

all:
		gcc -I include -Wunsequenced -o bin/rasm src/rasm.cpp
		gcc -I include -Wunsequenced -o bin/rex src/rex.cpp

all-debug:
		gcc -I include -Wunsequenced -o bin/rex-debug src/rex.cpp -DDEBUG
		gcc -I include -Wunsequenced -o bin/rasm-debug src/rasm.cpp -DDEBUG

test:
		./bin/rasm-debug tests/hello.rasm
		./bin/rex-debug tests/hello.rx
