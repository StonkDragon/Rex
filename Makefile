auto:
		gcc -arch x86_64 -o bin/rex rex.c

all:
		gcc -arch x86_64 -o bin/rex-x86 rex.c
		gcc -arch arm64 -o bin/rex-arm64 rex.c

x86:
		gcc -arch x86_64 -o bin/rex-x86 rex.c

arm64:
		gcc -arch arm64 -o bin/rex-arm64 rex.c

debug:
		gcc -o rex rex.c -DDEBUG

run:
		./rex main.rasm
