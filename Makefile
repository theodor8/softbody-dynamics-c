all: demo.out

clean:
	rm -f *.out


demo.out: Makefile ./src/*.c ./src/*.h
	gcc ./src/*.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -Wall -pedantic -o demo.out

leaks: demo.out
	leaks -atExit -- ./demo.out

run: demo.out
	./demo.out



.PHONY: all, clean, leaks, run

