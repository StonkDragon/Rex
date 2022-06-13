compile:
		gcc -o rex rex.c

debug:
		gcc -o rex rex.c -DDEBUG

run:
		./rex main.rasm
