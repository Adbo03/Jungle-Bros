#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <math.h>

/* Taille fenetre */ 
#define SCREEN_WIDTH 1250
#define SCREEN_HEIGHT 550

/* Vrai/Faux */
#define FALSE 0
#define TRUE 1

/* Pi */
#define PI 3.14159265359

/* MS per */
#define MS_PER_UPDATE 60
#define MS_PER_UPDATE_FOND 10

/* Coordonnees sol */
#define COORD_SOL 475

#define rgb2pixel(r,g,b) 			((long) ( (( uint8_t )r)<<8 | (( uint8_t )g))<<8 | (( uint8_t )b) )
#define Black						rgb2pixel(0  ,0   ,   0)
#define White						rgb2pixel(255,255 , 255)
#define Gray 						rgb2pixel(190, 190, 190)
#define LightGrey					rgb2pixel(211, 211, 211)
#define DarkGray					rgb2pixel(169, 169, 169)
#define NavyBlue					rgb2pixel(  0,   0, 128)
#define RoyalBlue					rgb2pixel( 65, 105, 225)
#define SkyBlue						rgb2pixel(135, 206, 235)
#define Blue						rgb2pixel(0  , 0  , 255)
#define LightBlue					rgb2pixel(173, 216, 230)
#define MidnightBlue				rgb2pixel(25 , 25 , 112)
#define Cyan 						rgb2pixel(  0, 255, 255)
#define LightCyan					rgb2pixel(224, 255, 255)
#define DarkCyan					rgb2pixel(0  , 139, 139)
#define Green						rgb2pixel(0  , 255,   0)
#define DarkGreen					rgb2pixel(  0, 100,   0)
#define LightGreen					rgb2pixel(144, 238, 144)
#define YellowGreen					rgb2pixel(154, 205,  50)
#define LightYellow					rgb2pixel(255, 255, 224)
#define IndianRed					rgb2pixel(205,  92,  92)
#define Sienna						rgb2pixel(160, 82 ,  45)
#define Beige						rgb2pixel(245, 245, 220)
#define Brown						rgb2pixel(165,  42,  42)
#define Orange						rgb2pixel(255, 165,   0)
#define DarkOrange					rgb2pixel(255, 140,   0)
#define LightCoral					rgb2pixel(240, 128, 128)
#define Pink						rgb2pixel(255, 192, 203)
#define LightPink					rgb2pixel(255, 182, 193)
#define Red							rgb2pixel(255, 0  ,   0)
#define DarkRed						rgb2pixel(139, 0  ,   0)
#define VioletRed					rgb2pixel(208,  32, 144)
#define Magenta						rgb2pixel(255,   0, 255)
#define DarkMagenta					rgb2pixel(139,   0, 139)
#define Violet						rgb2pixel(238, 130, 238)
#define DarkViolet					rgb2pixel(148,   0, 211)
#define Purple						rgb2pixel(160,  32, 240)
#define Gold1						rgb2pixel(255, 215,   0)
#define Gold2						rgb2pixel(238, 201,   0)
#define Gold3						rgb2pixel(205, 173,   0)
#define Gold4						rgb2pixel(139, 117,   0)

#define SPEED_WALK 300
#define SPEED_JUMP 400
#define SPEED_ENNEMI 350

/* Types d'ennemis */
#define TYPE_ARRAIGNE 0  
#define TYPE_OISEAU 1

/* Nombre maximum d'ennemis */
#define MAX_ENNEMIS 2 

typedef struct _Brick_menu{
    SDL_Rect brick_start;                /* pour gérer la position du bloc "start" */
    SDL_Rect select_start;               /* pour gérer la position du rectangle qui clignote (start)*/
    int selection_start;                 /* TRUE si "start" est selectionné */
    
    SDL_Rect brick_exit;                 /* pour gérer la position du bloc "exit" */
    SDL_Rect select_exit;                /* pour gérer la position du rectangle qui clignote (exit) */
    int selection_exit;                  /* TRUE si "exit" est sélectionné */
    
    int clignotement;                    /* Gestion du clignotement */
    SDL_Texture* texture;
} Brick_menu;

typedef struct _Effets_sonores{
    Mix_Chunk* select;                      // Effet selection du menu
    Mix_Chunk* menu;                        // Musique de fond Menu
    Mix_Chunk* jeu;                         // Musique de fond Jeu
    Mix_Chunk* saut;                        // Effet saut Personnage
    Mix_Chunk* marche;                      // Effet marche Personnage
    Mix_Chunk* arraigne;                    // Effet Arraigne
    Mix_Chunk* oiseau;                      // Effet Oiseau
    Mix_Chunk* WIN_SOUND_EFFECT;            // Musique Victoire
    Mix_Chunk* GAME_OVER_SOUND_EFFECT;      // Musique Defaite
    Mix_Chunk* Here_we_go;                  // Son Personnage debut jeu 
} Effets_sonores; 

typedef struct _End_game{
    SDL_Rect end_rect;
    int lose;
    int win;
    SDL_Texture* lose_texture;
    SDL_Texture* win_texture;
} End_game;

typedef struct _Caverne{
    /* Image 1 */
    SDL_Rect rect_g;                        /* Positions de la partie gauche de la caverne */
    SDL_Texture* texture_partie_gauche;     /* Texture de la partie gauche de la caverne (image) */
    
    /* Image 2 */
    SDL_Rect rect_d;                        /* Positions de la partie droite de la caverne */
    SDL_Texture* texture_partie_droite;     /* Texture de la partie droite de la caverne (image) */
    
    int pixel_target;                       /* Nombre de pixels que le personnage doit parcourir pour terminer la partie (appartion de la caverne) */
    int cpt_pixel;                          /* Nombre de pixels parcouru par le personnage */
    int move;                               /* TRUE si la caverne se deplace */
    int on_screen;                          /* TRUE si la caverne apparait completement a l'ecran */
} Caverne;

typedef struct _Arriere_plan{
    
    /* 
    Remarque: 
    L'arriere plan est decoupé en 4 parties pour assurer la continuite de l'image.
    */

    SDL_Rect fond_1;
    SDL_Texture* texture_1;
    SDL_Rect fond_2;
    SDL_Texture* texture_2;
    SDL_Rect fond_3;
    SDL_Texture* texture_3;
    SDL_Rect fond_4;
    SDL_Texture* texture_4;

} Arriere_plan;

/* init/close fenetre + ecran */
int init(SDL_Window** gWindow,SDL_Renderer** gRenderer, char* nameWindow);
void close(SDL_Window** gWindow,SDL_Renderer** gRenderer);

/* Gestion intéraction avec le menu du jeu */
void menu_control( SDL_Renderer* ecran, Brick_menu* bloc, int* quit, int* menu, Effets_sonores musique);
void Init_blocs(SDL_Renderer* ecran, SDL_Rect start_rect, SDL_Rect exit_rect, Brick_menu* blocs);

/* Initiliasation des images de fin de jeu */
void Init_end_game(SDL_Renderer* ecran, End_game* fin_jeu);

/* Dessine un rectangle */
void dessine_rect_plein(SDL_Renderer *gRenderer,SDL_Rect *rect,long c);

/* Pour le controle du temps des mises a jour */
double getCurrentTimeMS();

/* Initialise et permet de mettre à jour l'affichage du score */
void Init_police(SDL_Renderer* ecran, char* message, SDL_Texture** texture,char* mode, SDL_Rect* pos_rect);

/* Gestion des images */
SDL_Texture* creerTextureImg(SDL_Renderer* ecran,char* nom_file_bmp,int* size_w, int* size_h);
void dessine_Img_redim(SDL_Renderer* ecran,SDL_Texture* imgTexture,SDL_Rect imgRect);
void Init_image(SDL_Renderer* ecran, SDL_Texture** image_Texture, SDL_Rect* img_rect, char* file_img);

/* Gestion des effets sonores */
void Init_musique(Effets_sonores* musique);

/* Libère les alllocations liées aux musiques et effets sonores */
void free_audio(Effets_sonores* Musique);

/* Initialisation de la caverne de fin */
void Init_cave(SDL_Renderer* ecran, Caverne* cave);

/* Gestion du deplacement du fond */
void Deplacement_fond(Arriere_plan* decor, int vitesse);

/* Affichage de l'arriere plan */
void dessine_fond(SDL_Renderer* ecran, Arriere_plan decor);

/* Controle si le personnage rentre en collision avec les ennemis */
int checkCollision(SDL_Rect a, SDL_Rect b);

/* Detruit les textures */
void free_decor(Arriere_plan* decor);

#endif // _Settings_H
