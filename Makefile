build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lSDL2_ttf -lSDL2_image -o renderer -lm

run:
	./renderer

clean:
	rm renderer