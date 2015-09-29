project: game.o defn.o
	gcc game.o defn.o -Wall -o project

game.o: game.c defn.h
	gcc game.c -c -Wall

defn.o: defn.c defn.h
	gcc defn.c -c -Wall
