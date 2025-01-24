#ifndef _OBSTACLE_H 

#define _OBSTACLE_H

#ifndef _SETTINGS_H

#include "game_settings.h"

#endif //_SETTINGS_H

typedef struct _Obstacle{
    SDL_Rect rect;         // Position (coin superieur gauche)
    SDL_Texture* texture;  // image
    int x, y;              // position (au centre)
    int onScreen;
} Obstacle;

/* Initialisation obstacle */
void Init_obstacle(SDL_Renderer* ecran, Obstacle* obs);

/* Dessine l'obstacle sur l'ecran */
void dessine_obstacle(SDL_Renderer* ecran, Obstacle obs);

#endif // _OBSTACLE 