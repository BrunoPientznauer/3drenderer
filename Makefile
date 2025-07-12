build:
	gcc	-g	-Wall -std=c99 ./src/*.c -lSDL2 -lm	-o renderer	

run:
	./renderer

clean:
	rm renderer

profile:
	valgrind	--leak-check=full	./renderer