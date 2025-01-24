#ifndef _MARIO_H
#define _MARIO_H

#ifndef _SETTINGS_H

#include "game_settings.h"

#endif //_SETTINGS_H

#ifndef _OBSTACLE_H

#include "obstacle.h"

#endif // _OBSTACLE_H

#ifndef _ENNEMI_H

#include "ennemis.h"

#endif // _ENNEMI_H


typedef struct Personnage{
    SDL_Rect rect;
    int x;                              /* X position of the character */
    int y;                              /* Y position of the character */
    int previous_x;                     /* Memorisation of the last X position (for the display of "standing still" image) */
    int previous_y;                     /* Memorisation of the Y position before a jump (from the ground)*/
    int up;                             /* Indicates if the character is jumping */
    int down;                           /* Indicates if the character is falling (after a jump) */
    char dir;                           /* Indicates the orientation for the jump : 'd' (right); ' ' (up and down); 'g' (left)*/ 
    char previous_dir;                  /* To manage the "standing still" image (right/left) */
    int exit_menu;                      /* Indicates if the character is exiting the menu (start button) */
    int target_y;                       /* Height to reach during a jump */
    int alternance;
    int tps_up;
    int obs;                            /* TRUE if the character is on an obstacle */
    int exit_game;                      /* exit game (victory) */
    int isAlive;                        /* FALSE if the character is dead (game lost) */
    
    /* visuel */
    SDL_Texture* texture_d[2];
    SDL_Texture* texture_g[2];
    SDL_Texture* texture_arret[2];
    SDL_Texture* texture_saut[2];
    SDL_Texture* texture_mort;
    
} Personnage;

/* Manage the interaction between the player and the keyboard */
void control_keybord( SDL_Renderer* ecran, int* quit, Personnage* mario, End_game* fin_jeu, Effets_sonores musique, Obstacle* bloc, Caverne* cave, Arriere_plan* decor,Ennemi* ennemis[MAX_ENNEMIS]);

/* Initialize the main character */
void Init_personnage(SDL_Renderer* ecran, Personnage* mario);

/* Display the character on screen */
void dessine_personnage(SDL_Renderer* ecran, Personnage* mario);

/* Free the textures */
void free_mario(Personnage* mario);

#endif // _MARIO_H
