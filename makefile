all:
	gcc -g main.c parser.c scanner.c -o frontEnd
clean:
	rm -rf frontEnd