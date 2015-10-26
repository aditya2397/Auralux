CFLAGS = -Wall -pedantic -Werror
LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lm
OBJS   = draw.o entity.o init.o input.o graphics.o main.o functions.o player.o cpu.o
PROG = project
CXX = gcc

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
%.o: src/%.c src/%.h src/defs.h src/structs.h
	$(CXX) $(CFLAGS) -c -s $<

# linking the program.
$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $(PROG) $(LFLAGS)

# cleaning everything that can be automatically recreated with "make".
clean:
	rm $(PROG) *.o
