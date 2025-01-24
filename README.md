# Jungle-Bros
The idea behind this project is to code a game in C inspired by the game franchise Super Mario Bros. This was a group project. Before going further, it's only right to state the names of the two guys who contributed in coding this game : @Jack-972 and @Nardey123 (github profiles).

This game uses the same ingredients as Super Mario : a main character who must try to avoid obstacles and enemies to win the game (there is no princess rescuing in this game. Sorry !). If the character, lets call him Junko, were to EVEN touch an enemy, the game and all hope would be lost.  

We coded this game using the SDL2 library for the display, the SDL2_Mixer library for the sound effects and finally the SDL2_TTF library. To organise our project, we decided to separate the different aspects of the game into files :
- main.c which I'm sure you would have guessed stores the main function and allows to run the program.
- game_settings.c (with it's header file game_settings.h) which handles all functions related to the game itself (creation of the window, managing the game menu, initialization of the musics,...)
- obstacle.c (with it's header file obstacle.h) which manage all different aspects related to the obstacles (movement, initialization, display,...)
- mario.c (with it's header file mario.h) which handles all different aspects related to Junko (movement, display, initialization, ...)
- ennemis.c (with it's header file ennemis.c) which handles all different aspects related to the ennemies (movement, display, initialization,...)

To make things easier, a Makefile is available for the compilation.

USING THE MAKEFILE :

For the game to compile, the following libraries must be dowloaded (best not through the terminal though it might work):
- SDL2_mixer-2.6.3
- SDL2_ttf-2.20.2
- SDL2-2.26.4
  
Once dowloaded, the folders of the libraries must be placed in the SAME FOLDER where the others files are stored (espacially main.c) and, for each library, the .dll file (in the lib folder) must be moved to the main folder (where all the files are located, i.e. the same folder were the libraries are stored).

Of course, there are different ways to compile and run the program as long as you install the required libraries. The "tuto" above matters only if you wish to compile it using the makefile.

ENJOY ! 

PS : The game was coded by French guys so the name of some variables might look odd ^^ (thanksfully, the comments are in english for you to better understand how the game was developed).
