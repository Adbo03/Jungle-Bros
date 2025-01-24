# Jungle-Bros
The idea behind this project is to code a game in C inspired by the game franchise Super Mario Bros. This was a group project. Before going further, it's only right to state the names of the two guys who contributed in coding this game : @Jack-972 and @Nardey123 (github profiles).

This game uses the same ingredients as Super Mario : a main character who must try to avoid obstacles and enemies to win the game (there is no princess rescuing in this game. Sorry !). If the character, lets call him Junko, were to EVEN touch an enemy, the game and all hope would be lost.  

We coded this game using the SDL2 library for the display, the SDL2_Mixer library for the sound effects and finally the SDL2_TTF library (which you'll all find attached).To organise our project, we decided to separate the different aspect of the game into files :
- main.c which I'm sure you would have guessed stores the main function and allows to run the program.
- game_settings.c (with it's header file game_settings.h) which handles all functions related to the game itself (opening of the window, managing the game menu, initialisation of the musics,...)
- obstacle.c (with it's header file obstacle.h) which manage all different aspects related to the obstacles (movement, initialization, display,...)
- mario.c (with it's header file mario.h) which handles all different aspects related to Junko (movement, display, initialization, ...)
- ennemis.c (with it's header file ennemis.c) which handles all different aspects related to the ennemies (movement, display, initialization,...)

To make things easier, a Makefile is available for the compilation.
