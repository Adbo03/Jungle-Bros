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
    int x;                              /* position en x du personnage */
    int y;                              /* position en y du personnage */
    int previous_x;                     /* mémorise le x précédent (affichage image arret) */
    int previous_y;                     /* mémorise le y avant un saut (depuis le sol)*/
    int up;                             /* indique si le personnage saute */
    int down;                           /* indique si le personnage retombe sur le sol */
    char dir;                           /* indique le sens de déplacement pour le saut : 'd' (droite); ' ' (sur place); 'g' (gauche)*/ 
    char previous_dir;                  /* Pour gerer l'affichage de l'arret (droit/gauche) */
    int exit_menu;                      /* sortie du menu (appui start) */
    int target_y;                       /* Hauteur à atteindre lors d'un saut */
    int alternance;
    int tps_up;
    int obs;                            /* Actif si le personnage est sur un obstacle */
    int exit_game;                      /* sortie du jeu (partie gagnee) */
    int isAlive;                        /* FALSE si le personnage est mort (partie perdue) */
    
    /* visuel */
    SDL_Texture* texture_d[2];
    SDL_Texture* texture_g[2];
    SDL_Texture* texture_arret[2];
    SDL_Texture* texture_saut[2];
    SDL_Texture* texture_mort;
    
} Personnage;

/* Gère l'interaction du joueur avec le clavier */
void control_keybord( SDL_Renderer* ecran, int* quit, Personnage* mario, End_game* fin_jeu, Effets_sonores musique, Obstacle* bloc, Caverne* cave, Arriere_plan* decor,Ennemi* ennemis[MAX_ENNEMIS]);

/* Initialise le personnage principal */
void Init_personnage(SDL_Renderer* ecran, Personnage* mario);

/* Dessine le personnage sur l'ecran */
void dessine_personnage(SDL_Renderer* ecran, Personnage* mario);

/* Desalloue les textures du personnage */
void free_mario(Personnage* mario);

#endif // _MARIO_H