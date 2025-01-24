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

/*Window size */ 
#define SCREEN_WIDTH 1250
#define SCREEN_HEIGHT 550

/* TRUE/FALSE */
#define FALSE 0
#define TRUE 1

/* Pi */
#define PI 3.14159265359

/* MS per */
#define MS_PER_UPDATE 60
#define MS_PER_UPDATE_FOND 10

/* Ground coordinates (in X) */
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

/* Types of enemies */
#define TYPE_ARRAIGNE 0      // Spider
#define TYPE_OISEAU 1        // Bird

/* Maximum number of enemies */
#define MAX_ENNEMIS 2 

typedef struct _Brick_menu{
    SDL_Rect brick_start;                /* manage the position of the brick "start" */
    SDL_Rect select_start;               /* manage the position of the selection rectangle (start) */
    int selection_start;                 /* TRUE if "start" is selected */
    
    SDL_Rect brick_exit;                 /* manage the position of the brick "exit" */
    SDL_Rect select_exit;                /* manage the position of the selection rectangle (exit) */
    int selection_exit;                  /* TRUE if "exit" is selected */
    
    int clignotement;                    /* Manage the blinking */
    SDL_Texture* texture;
} Brick_menu;

typedef struct _Effets_sonores{
    Mix_Chunk* select;                      // Selection sound for the game menu
    Mix_Chunk* menu;                        // music of game menu
    Mix_Chunk* jeu;                         // game music
    Mix_Chunk* saut;                        // jump sound for the character
    Mix_Chunk* marche;                      // walking sound for the character
    Mix_Chunk* arraigne;                    // spider sound effect 
    Mix_Chunk* oiseau;                      // bird sound effect
    Mix_Chunk* WIN_SOUND_EFFECT;            // Win music
    Mix_Chunk* GAME_OVER_SOUND_EFFECT;      // Game over sound effect
    Mix_Chunk* Here_we_go;                  // Character sound (beginning of a game) 
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
    SDL_Rect rect_g;                        /* Position of the left part of the cave */
    SDL_Texture* texture_partie_gauche;     /* Texture of the left part of the cave (image) */
    
    /* Image 2 */
    SDL_Rect rect_d;                        /* Position of the right part of the cave */
    SDL_Texture* texture_partie_droite;     /* Texture of the right part of the cave (image) */
    
    int pixel_target;                       /* Number of pixels the character has to walk to win a game (display of the cave) */
    int cpt_pixel;                          /* Number of pixels walked by the character */
    int move;                               /* TRUE is the cave is appearing */
    int on_screen;                          /* TRUE if the cave is fully displayed on screen */
} Caverne;

typedef struct _Arriere_plan{
    
    /* 
    NOTE: 
    The background is cut into 4 parts to insure a smooth movement effect.
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

/* init/close window + screen */
int init(SDL_Window** gWindow,SDL_Renderer** gRenderer, char* nameWindow);
void close(SDL_Window** gWindow,SDL_Renderer** gRenderer);

/* Manage the interaction with the game menu */
void menu_control( SDL_Renderer* ecran, Brick_menu* bloc, int* quit, int* menu, Effets_sonores musique);
void Init_blocs(SDL_Renderer* ecran, SDL_Rect start_rect, SDL_Rect exit_rect, Brick_menu* blocs);

/* Initialization of the endgame images */
void Init_end_game(SDL_Renderer* ecran, End_game* fin_jeu);

/* Draw a rectangle */
void dessine_rect_plein(SDL_Renderer *gRenderer,SDL_Rect *rect,long c);

/* Get the current time in ms (used to control the updates) */
double getCurrentTimeMS();

/* Initialize the display of the text for the game menu */
void Init_police(SDL_Renderer* ecran, char* message, SDL_Texture** texture,char* mode, SDL_Rect* pos_rect);

/* Manage the creation, initialization and display of the textures (images) */
SDL_Texture* creerTextureImg(SDL_Renderer* ecran,char* nom_file_bmp,int* size_w, int* size_h);
void dessine_Img_redim(SDL_Renderer* ecran,SDL_Texture* imgTexture,SDL_Rect imgRect);
void Init_image(SDL_Renderer* ecran, SDL_Texture** image_Texture, SDL_Rect* img_rect, char* file_img);

/* Manage the sound effects */
void Init_musique(Effets_sonores* musique);

/* Free the memory allocated for the musics and sounds effects */
void free_audio(Effets_sonores* Musique);

/* Initialize the end game cave */
void Init_cave(SDL_Renderer* ecran, Caverne* cave);

/*Manage the movement of the background as the character walks */
void Deplacement_fond(Arriere_plan* decor, int vitesse);

/* Manage the background display */
void dessine_fond(SDL_Renderer* ecran, Arriere_plan decor);

/* Check if the character bump into an enemy  */
int checkCollision(SDL_Rect a, SDL_Rect b);

/* Destroys the textures */
void free_decor(Arriere_plan* decor);

#endif // _Settings_H
