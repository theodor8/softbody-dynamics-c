demo.out: ./src/*.c
	gcc ./src/*.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -Wall -pedantic -o demo.out

leaks: demo.out
	leaks -atExit -- ./demo.out

run: demo.out
	./demo.out