/*  Auralux game
    Copyright (C) 2015 Aditya Barve
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

extern void init(char *);
extern void cleanup(void);
extern void getInput(void);
extern void draw(void);
extern void initPlayer(void);
extern void doPlayer(void);
extern void doEntities(int);
extern void delay(unsigned int);
extern void clearEntities(void);

extern void initLevel(char *);
extern void clearInput();
extern void cpuThink();
extern char *get_timestamp();
extern void give_user_info();
extern void init_level_names();

extern int active_sun_colors[NO_OF_COLORS / 2];
extern unsigned int no_of_dots[NO_OF_COLORS / 2];

int main(int argc, char **argv)
{
	unsigned int frameLimit = SDL_GetTicks() + MIN_TICK_DELAY;
	int i, active_colors;
	char *filename = (char *)malloc(100);
	char *graph_title = (char *)malloc(100);
	char temp[100];
	int step = 0;
	int max_no_of_dots = 0, winner_no_of_dots;
	unsigned int seconds_of_gameplay = 0;
	FILE *game_scores;
	char **gnuplot_commands;
	FILE *gnuplotPipe;

	init_level_names();
 	if(argc < 2) {
 		give_user_info();
 		return 0;
	}
	sprintf(graph_title, "Game Results: ");
	filename = get_timestamp();
	strcat(filename, "Auralux.dat");
	game_scores = fopen(filename, "w");

	if(!game_scores) {
		perror("game_scores pointer");
	}
	/* Start up SDL */
	init("Auralux 1");
	/* Call the cleanup function when the program exits */
	atexit(cleanup);
	clearInput();
	initLevel(argv[1]);
	while (1) {
		active_colors = 0;
		for(i = 0; i < NO_OF_COLORS / 2; i++) {
			if(active_sun_colors[i]) {
				active_colors++;
			}
		}
		if(active_colors == 1 || !active_sun_colors[BLUE / 2]) {
			break;
		}
		/* Get the input */
		getInput();
		/* Update player and entities */
		doEntities(step);
		doPlayer();
		/* CPU opponents decide strategy */
		if(step % (1000 / MIN_TICK_DELAY) == 0) {
			cpuThink();
			/* add data to game scores */
			fprintf(game_scores, "%d\t", seconds_of_gameplay++);
			for(i = 0; i < NO_OF_COLORS / 2; i++) {
				fprintf(game_scores, "%d\t", no_of_dots[i]);
				if(no_of_dots[i] > max_no_of_dots) {
					max_no_of_dots = no_of_dots[i];
				}
			}
			fprintf(game_scores, "\n");
		}
		/* Draw everything */
		draw();
		/* Sleep briefly to stop sucking up all the CPU time */
		delay(frameLimit);
		frameLimit = SDL_GetTicks() + MIN_TICK_DELAY;
		step--;
	}
	fclose(game_scores);
	game_scores = fopen(filename, "r");
	if(!game_scores) {
		perror("game_scores pointer");
	}
	fscanf(game_scores, "%d", &step); /* time column value skipped */
	if(active_sun_colors[BLUE / 2]) {
		strcat(graph_title, "You won!");
	}
	else {
		strcat(graph_title, "You lost!");
	}
	/* gnuplot commands begin here */
	gnuplot_commands = (char **)malloc(10 * sizeof(char *));
	for(i = 0; i < 10; i++) { /* assuming that 10 gnuplot commands are required */
		gnuplot_commands[i] = (char *)malloc(1000);
	}
	/* set title of the graph */
	strcpy(gnuplot_commands[0], "set title font \", 20\" \"");
	strcat(gnuplot_commands[0], graph_title);
	strcat(gnuplot_commands[0], "\"");
	/* set xrange*/
	strcpy(gnuplot_commands[1], "set xrange[0:");
	sprintf(temp, "%d", seconds_of_gameplay);
	strcat(gnuplot_commands[1], temp);
	strcat(gnuplot_commands[1], "]");
	/* set yrange */
	strcpy(gnuplot_commands[2], "set yrange[0:");
	sprintf(temp, "%d", (int)(1.3 * max_no_of_dots)); /* extra space at the top of the graph */
	strcat(gnuplot_commands[2], temp);
	strcat(gnuplot_commands[2], "]");
	/* set xlabel */
	strcpy(gnuplot_commands[3], "set xlabel 'time (in seconds)'");
	/* set ylabel */
	strcpy(gnuplot_commands[4], "set ylabel 'dot population'");
	/* set line styles */
	strcpy(gnuplot_commands[5], "set style line 1 lt 1 lw 2 pt 3 lc rgb \"blue\"");	
	strcpy(gnuplot_commands[6], "set style line 2 lt 1 lw 2 pt 3 lc rgb \"orange\"");	
	strcpy(gnuplot_commands[7], "set style line 3 lt 1 lw 2 pt 3 lc rgb \"green\"");	
	strcpy(gnuplot_commands[8], "set style line 4 lt 1 lw 2 pt 3 lc rgb \"purple\"");
	/* decide which colors to plot on graph */
	strcpy(gnuplot_commands[9], "plot ");
	for(i = 0; i < NO_OF_COLORS / 2; i++) {
		fscanf(game_scores, "%d", &winner_no_of_dots); /* time column value skipped */
		if(!winner_no_of_dots) {
			continue;
		}
		strcat(gnuplot_commands[9], "'");
		strcat(gnuplot_commands[9], filename);
		strcat(gnuplot_commands[9], "'");
		strcat(gnuplot_commands[9], " using 1:");
		sprintf(temp, "%d ", 2 + i); /* dot counts are from column 2 onwards */
		strcat(gnuplot_commands[9], temp);
		strcat(gnuplot_commands[9], "title \"");
		switch(i * 2 + 1) {
			case BLUE:
				strcpy(temp, "You");
				break;
			case ORANGE:
				strcpy(temp, "CPU1");
				break;
			case GREEN:
				strcpy(temp, "CPU2");
				break;
			case PURPLE:
				strcpy(temp, "CPU3");
				break;
		}
		strcat(gnuplot_commands[9], temp);
		strcat(gnuplot_commands[9], "\" ");
		strcat(gnuplot_commands[9], "smooth cspline ");
		strcat(gnuplot_commands[9], "w l ls ");
		sprintf(temp, "%d, ", 1 + i); /* line styles are from 1 onwards */
		strcat(gnuplot_commands[9], temp);
	}
	gnuplotPipe = popen("gnuplot -persistent", "w");
	if(!gnuplotPipe) {
		perror("gnuplotPipe pointer");
	}
	for (i = 0; i < 10; i++)
	{
		printf("%s\n", gnuplot_commands[i]);
		fprintf(gnuplotPipe, "%s\n", gnuplot_commands[i]); /* send commands to gnuplot one by one */
	}	
	/* Exit the program */
	exit(0);
}

