#ifndef _OBSTACLE_H 

#define _OBSTACLE_H

#ifndef _SETTINGS_H

#include "game_settings.h"

#endif //_SETTINGS_H

typedef struct _Obstacle{
    SDL_Rect rect;         // Position (upper left corner)
    SDL_Texture* texture;  // image
    int x, y;              // position (center)
    int onScreen;
} Obstacle;

/* Initialization of the obstacle */
void Init_obstacle(SDL_Renderer* ecran, Obstacle* obs);

/* Display the obstacle on screen */
void dessine_obstacle(SDL_Renderer* ecran, Obstacle obs);

#endif // _OBSTACLE 
