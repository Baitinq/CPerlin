all: CPerlin

CPerlin: main.c perlin.c include/perlin.h
	gcc -lSDL2 -lm main.c perlin.c -o CPerlin

clean:
	rm CPerlin
