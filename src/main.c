/* This tutorial will open a 640 x 480 screen,
** load an image and allow the user to control it
** using the arrow keys. Pressing Space will fire
** a bullet. Pressing escape
** or closing the window will exit the program
*/

#include "main.h"

extern void init(char *);
extern void cleanup(void);
extern void getInput(void);
extern void draw(void);
extern void initPlayer(void);
extern void doPlayer(void);
extern void doEntities(int);
extern void loadAllSprites(void);
extern void loadAllSounds(void);
extern void delay(unsigned int);
extern void clearEntities(void);

extern void initEntities();
extern void initLevel();
extern void clearInput();
extern void cpuThink();
extern char *get_timestamp();

extern int active_sun_colors[NO_OF_COLORS / 2];
extern unsigned int no_of_dots[NO_OF_COLORS / 2];

int main(int argc, char *argv[])
{
	unsigned int frameLimit = SDL_GetTicks() + MIN_TICK_DELAY;
	int i, active_colors;
	char *filename = (char *)malloc(100);
	char *graph_title = (char *)malloc(100);
	char temp[100];
	int step = 0;
	int winner_no_of_dots;
	unsigned int seconds_of_gameplay = 0;
	FILE *game_scores;
	char **gnuplot_commands;
	FILE * gnuplotPipe;
	/* Start up SDL */
	sprintf(graph_title, "Game Results: ");
	filename = get_timestamp();
	strcat(filename, "Auralux.dat");
	game_scores = fopen(filename, "w");
	if(!game_scores) {
		perror("game_scores pointer");
	}
	init("Auralux 1");
	/*
	printf("init\n");
	*/
	/* Call the cleanup function when the program exits */
	
	atexit(cleanup);
	/*
	printf("atexit\n");
	*/	
	/* Load all the sprites */
	
	loadAllSprites();
	/*
	printf("load all sprites\n");
	*/
	/* Load all the sounds
	
	loadAllSounds();*/
	
	/* initialise sun */
	
	clearInput();
	initEntities();
	/*
	printf("init entities\n");
	*/
	initLevel();
	/*
	printf("init level\n");
	*/
	/* Loop indefinitely for messages */
	
	while (1/*step*/)
	{
		active_colors = 0;
		for(i = 0; i < NO_OF_COLORS / 2; i++) {
			if(active_sun_colors[i]) {
				active_colors++;
			}
		}
		/*
		printf("no of active colors : %d\n", active_colors);
		*/
		if(active_colors == 1) {
			break;
		}
		/*
		printf("step : %d\n", step);
		*/
		/* Get the input */
		
		getInput();
		/*
		printf("get input\n");
		*/
		/* Update the entities */
		
		doEntities(step);
		doPlayer();
		/*
		printf("do entities\n");
		printf("do player\n");
		*/
		/* CPU opponents decide strategy */
		if(step % (1000 / MIN_TICK_DELAY) == 0) {
			cpuThink();
			/*
			printf("CPU has thought about it\n");
			*/
			/* add data to game scores */
			fprintf(game_scores, "%d\t", seconds_of_gameplay++);
			for(i = 0; i < NO_OF_COLORS / 2; i++) {
				/*
				if(!active_sun_colors[i]) {
					no_of_dots[i]>>= 1;
				}
				*/
				fprintf(game_scores, "%d\t", no_of_dots[i]);
				printf("%d\t", no_of_dots[i]);
			}
			fprintf(game_scores, "\n");
			printf("\n");
		}
		
		/* Draw everything */
		
		draw();
		/*
		printf("draw\n");
		*/
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
	fscanf(game_scores, "%d", &winner_no_of_dots); /* time column value skipped */
	/* game ended, someone won */
	for(i = 0; i < NO_OF_COLORS / 2; i++) {
		if(active_sun_colors[i]) {
			winner_no_of_dots = no_of_dots[i];
			switch(i) {
				case 0:
					strcat(graph_title, "BLUE ");
					break;
				case 1:
					strcat(graph_title, "ORANGE ");
					break;
				case 2:
					strcat(graph_title, "GREEN ");
					break;
				case 3:
					strcat(graph_title, "PURPLE ");
					break;
			}
			break;
		}
	}
	strcat(graph_title, "won!");
	
	/* gnuplot commands begin here */
	gnuplot_commands = (char **)malloc(10 * sizeof(char *));
	for(i = 0; i < 10; i++) { /* assuming that 10 gnuplot commands are required */
		gnuplot_commands[i] = (char *)malloc(1000);
		/*
			char **gnuplot_commands = {"set title \"", "set xrange[0:", "set yrange[0:", "plot "};

			i = 0 : graph title
			i = 1 : xrange
			i = 2 : yrange
			i = 3 : actual plot command
		*/
	}
	/* set title of the graph */
	strcpy(gnuplot_commands[0], "set title \"");
	strcat(gnuplot_commands[0], graph_title);
	strcat(gnuplot_commands[0], "\"");
	/* set xrange*/
	strcpy(gnuplot_commands[1], "set xrange[0:");
	sprintf(temp, "%d", seconds_of_gameplay);
	strcat(gnuplot_commands[1], temp);
	strcat(gnuplot_commands[1], "]");
	/* set yrange */
	strcpy(gnuplot_commands[2], "set yrange[0:");
	sprintf(temp, "%d", (int)(1.1 * winner_no_of_dots)); /* extra space at the top of the graph */
	printf("winner dots : %d", (int)(1.1 * winner_no_of_dots)); /* extra space at the top of the graph */
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
		if(!winner_no_of_dots) { /* when t = 1, there were no dots of that color. so there were no suns of that color. so don't plot this column */
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
				strcpy(temp, "BLUE");
				break;
			case ORANGE:
				strcpy(temp, "ORANGE");
				break;
			case GREEN:
				strcpy(temp, "GREEN");
				break;
			case PURPLE:
				strcpy(temp, "PURPLE");
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

