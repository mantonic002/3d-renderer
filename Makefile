build:
	gcc-13 -Wall -std=c99 ./src/*.c -lSDL2 -lSDL2_image -o renderer -lm

debug:
	gcc-13 -Wall -std=c99 -ggdb ./src/*.c -lSDL2 -lSDL2_image -o renderer-deb -lm

run:
	./renderer

clean:
	rm renderer-deb
	rm renderer