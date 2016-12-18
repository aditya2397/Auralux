CFLAGS = -Wall -pedantic -Werror
LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_gfx -lm
OBJS   = draw.o entity.o init.o input.o main.o functions.o player.o cpu.o
PROG = project
CXX = gcc

# top-level rule to create the program.
all: $(PROG)
	$(info )
	$(info README)
	$(info type "make install" to install all necessary libraries)
	$(info type "make clean" to delete all compiled files)
	$(info type "make clean_data" to remove all .dat files created for plotting graphs)
	$(info type "make count_lines" to count lines of source code)

# compiling other source files.
%.o: src/%.c src/%.h src/defs.h src/structs.h
	$(CXX) $(CFLAGS) -c -s -g $<

# linking the program.
$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $(PROG) $(LFLAGS)

# cleaning everything that can be automatically recreated with "make".
clean:
	rm $(PROG) *.o src/*~ *~

install:
	sudo apt-get install gnuplot libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev

count_lines:
	wc -l src/*.c src/*.h
	
# removes all .dat files used to plot graphs
clean_data:
	rm *.dat
