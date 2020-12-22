all: console processor

console: console.c
	gcc -o console console.c

processor: processor.c
	gcc -o processor processor.c


