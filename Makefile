all:
		gcc -arch x86_64 -o rex-x86 rex.c
		gcc -arch arm64 -o rex-arm64 rex.c

x86:
		gcc -arch x86_64 -o rex-x86 rex.c

arm64:
		gcc -arch arm64 -o rex-arm64 rex.c

debug:
		gcc -o rex rex.c -DDEBUG

run:
		./rex main.rasm
