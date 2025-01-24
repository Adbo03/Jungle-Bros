#ifndef _ENNEMI_H
#define _ENNEMI_H

#ifndef _SETTINGS_H
#include "game_settings.h"
#endif // _SETTINGS_H

#ifndef _OBSTACLE_H
#include "obstacle.h"
#endif // _OBSTACLE_H

// Structure représentant un ennemi
typedef struct _Ennemi{
    SDL_Rect rect;
    SDL_Texture** textures;     // Tableau dynamique de textures
    int textureCount;           // Compte le nombre de textures necessaire pour l'animation de l'ennemis
    float x, y;                 // Position de l'ennemi
    float speed;                // Vitesse de déplacement de l'ennemi
    float speed_start;
    int isAlive;                // État de l'ennemi : 1 = vivant (affiché), 0 = mort (hors de l'ecran)
    int type;                   // Type de l'ennemi pour des comportements différents
    int currentFrame;           // Frame actuel
    int animationDelay;         // Délai en nombre d'itérations
    int animationCounter;       // Compteur pour gérer le délai
    double jumpTimer;           // Temps écoulé depuis le dernier saut
    int isJumping;              // Indique si l'araignée est en train de sauter
    double jumpStartX;          // Position X de départ pour le saut
    double jumpHeight;          // Hauteur maximale du saut
} Ennemi;

// Initialise un ennemi avec ses paramètres de base
Ennemi* initEnnemi(SDL_Renderer* ecran, int type);

// Met à jour la position et l'état de l'ennemi
void updateEnnemi(Ennemi* ennemi[MAX_ENNEMIS],double elapsed,Effets_sonores* musique, Caverne cave, Obstacle obs);

/* Dessine les ennemis */
void dessine_Ennemis(Ennemi* ennemi,SDL_Renderer* ecran);

//Change l'image de l'ennemi à chaque appel
void updateAnimation(Ennemi* ennemi);

/* Verifie si un ennemi est affiché à l'ecran */
int check_Alive(Ennemi* ennemis[MAX_ENNEMIS]);

/* Desalloue les textures et l'ennemi */
void free_ennemis(Ennemi** ennemi);

#endif // _ENNEMI_H
