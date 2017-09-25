compile:
	clang main.c -o main && ./main

s:
	clang sscanf.c -o sscanf && ./sscanf

gdb:
	clang -ggdb main.c -o main && gdb main