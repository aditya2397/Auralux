Title : AURALUX game

Name : Aditya Barve

MIS : 111403080

Description (brief / 100 words) :

	This is an emulation of the Windows/Android real time strategy game called "Auralux". The 'config' folder contains 7 default levels and a default color scheme, which can be modified. Display of graphics is done using the SDL library. There is one (human) player and multiple CPU opponents, with adaptive heuristics (timid, developing and aggressive). Each player can occupy empty suns, upgrade existing suns or conquer enemy suns. The CPU opponents try to defeat the player as well as each other. When the player wins or gets knocked out, the game ends and the program pipes commands to gnuplot to plot a graph of 'dot population' versus 'time'.

Description (elaborate /  250 words) :

	First, SDL is initialised. Then the coordinates, colors and current levels of each sun are read from a .txt file in the 'config' folder corresponding to the level selected. A filename for the graph data file is generated using the computer's timestamp. In the game while loop, we take user input, have all the entities (dots, suns and select ring) carry out their tasks and then draw all entities. To conserve memory, the CPU opponents think and command their units every second, instead of during every loop cycle. Every second, the number of dots of each color are also recorded in the graph data file.

	The CPU opponents use graph theory (adjacency matrix) to decide which suns to attack. The CPU can decide to attack a sun (priority : nearest, weakest, empty etc.) or decide to help out a friendly sun in healing or attacking. As a CPU covers more territory on the field, it reduces defensive measures and becomes more willing to attack enemy suns with more units flocked around them. The non-empty suns produce a certain number of dots per second. A sun of level 2 produces twice as many dots as a sun of level 1. A sun of level 3 produces thrice as many dots as a sun of level 1.

	When the player wins or gets knocked out, the program pipes commands to gnuplot to plot a graph using the data gathered per second (of no. of dots of each color) and displays whether the player won or lost.

Game Instructions :

type "./project" to see a list of level indices
type "./project level_index" where level_index corresponds to the level you want to play

1) User controls :
click on a sun to select it (visual aid : white select ring will appear around the sun)
click on another sun to send dots from 1st sun to this sun (visual aid : select ring will float to 2nd sun and disappear)
to conquer an enemy sun, first reduce health of enemy to zero to make it an empty sun. then increase its health to 100 again (as your own color)
send units to a friendly sun to heal it or upgrade it (if already healthy)
dots surrounding a sun can upgrade the same sun (double click on that sun)
press ESC to quit the level

2) Basic info :
by default, you control the BLUE units. you can change the color scheme by changing hex rgb values in "config\colors.txt"
suns will constantly generete some units (dots) per second
larger suns will have a faster generation rate
number of gray bands around each sun, signify how much further the sun be upgraded to produce more dots
if a sun has 1 gray band around it, it can be upgraded 1 time
if a sun has 2 gray bands around it, it can be upgraded 2 times
suppose that a sun at level 1 produces 'x' units per second.
then a level 2 sun will produce '2x' units per second
and a level 3 sun will produce '3x' units per second
when sun's health is not full, a red health bar will be displayed below it
when sun's upgrade is not complete, a colored bar (corresponding to color of that sun) will appear below it

IMP : human player's suns will individually protect themselves when they are under attack, but have no further automated offense or defense
