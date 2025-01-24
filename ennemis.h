#ifndef _ENNEMI_H
#define _ENNEMI_H

#ifndef _SETTINGS_H
#include "game_settings.h"
#endif // _SETTINGS_H

#ifndef _OBSTACLE_H
#include "obstacle.h"
#endif // _OBSTACLE_H

// Structure of an enemy
typedef struct _Ennemi{
    SDL_Rect rect;
    SDL_Texture** textures;     // Dynamic tab of textures
    int textureCount;           // Count the number of textures required for the animation of the enemy
    float x, y;                 // Position of the enemy
    float speed;                // Movement speed of the enemy
    float speed_start;
    int isAlive;                // State of the enemy : 1 = alive (display), 0 = dead (out of screen)
    int type;                   // Type of enemy (spider or bird)
    int currentFrame;           // current frame for the display
    int animationDelay;         // Delay (number of loops)
    int animationCounter;       // Counter to manage the delay
    double jumpTimer;           // elapsed time since the last jump
    int isJumping;              // Indicates if the spider is currently jumping
    double jumpStartX;          // X position at the beginning of a jump
    double jumpHeight;          // Maximal height for a jump
} Ennemi;

// Initialize an enemy with it's parameters by default
Ennemi* initEnnemi(SDL_Renderer* ecran, int type);

// Update the position and state of an enemy
void updateEnnemi(Ennemi* ennemi[MAX_ENNEMIS],double elapsed,Effets_sonores* musique, Caverne cave, Obstacle obs);

// Display of the enemy 
void dessine_Ennemis(Ennemi* ennemi,SDL_Renderer* ecran);

// Change the image of the enemy at every call (movement effect)
void updateAnimation(Ennemi* ennemi);

// Check if an enemy is displayed on screen
int check_Alive(Ennemi* ennemis[MAX_ENNEMIS]);

// Destroys the textures and free the memory allocated for the enemy
void free_ennemis(Ennemi** ennemi);

#endif // _ENNEMI_H
