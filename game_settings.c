#include <math.h>
#include "game_settings.h"
#include "mario.h"

int init(SDL_Window** gWindow,SDL_Renderer** gRenderer, char* nameWindow){
	
        /* Initialization flag */
	int success = TRUE;

	/* SDL Initialization */
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = FALSE;
	}
	else{
		
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		/* Create the window */
		*gWindow = SDL_CreateWindow(nameWindow, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( *gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = FALSE;
		}
		else{
			
			*gRenderer = SDL_CreateRenderer( *gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( *gRenderer == NULL ){
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = FALSE;
			}
			else{
				/* Initialization of the color of the screen */
				SDL_SetRenderDrawColor( *gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}

	return success;
}


void close(SDL_Window** gWindow,SDL_Renderer** gRenderer){
	/* Destruction of the window */
	SDL_DestroyRenderer( *gRenderer );
	SDL_DestroyWindow(* gWindow );
	*gWindow = NULL;
	*gRenderer = NULL;

	/* Close SDL */
	SDL_Quit();
}

/* Manage the interaction with the game menu */
void menu_control( SDL_Renderer* ecran, Brick_menu* bloc, int* quit, int* menu, Effets_sonores musique){
    SDL_Event e;

    while(SDL_PollEvent( &e ) && menu){
        
        /* If a key is pressed */
        if( e.type == SDL_KEYDOWN){
            
            /* and let go */
            if (e.key.repeat == 0){ 

                switch( e.key.keysym.sym )
                {   
                    /* Right */
                    case SDLK_RIGHT:   
                        if(!bloc->selection_exit){
                            bloc->selection_exit = TRUE; 
                            bloc->selection_start = FALSE;  
                            bloc->clignotement = 1;
                            Mix_PlayChannel(1,musique.select, 0);
                        }                                                  
                    break;

                    /* Left */
                    case SDLK_LEFT:  
                        if(!bloc->selection_start){
                            bloc->selection_start = TRUE;
                            bloc->selection_exit = FALSE;
                            bloc->clignotement = 1;
                            Mix_PlayChannel(1,musique.select, 0);
                        }                                                      
                    break;

                    /* Return key */
                    case SDLK_RETURN:
                        if(bloc->selection_exit){
                            *quit = TRUE;
                            *menu = FALSE;              // To inidicates if we exit the menu
                        }
                        else if(bloc->selection_start){
                            *menu = FALSE;
                            bloc->selection_start = FALSE;
                        }
                    break;

                    /* To quit the game (Escape key) */
                    case SDLK_ESCAPE:
                        *quit=TRUE;
                    break;
                }
            }
        }
    }        

}

/* Initialization of the bricks (menu) */
void Init_blocs(SDL_Renderer* ecran, SDL_Rect start_rect, SDL_Rect exit_rect, Brick_menu* blocs){
    blocs->selection_start = FALSE;
    blocs->selection_exit = FALSE;
    blocs->brick_start = start_rect;
    blocs->brick_exit = exit_rect;

    /* Brick start */
    blocs->brick_start.y -= blocs->brick_start.h/2;
    blocs->brick_start.x -= blocs->brick_start.w/2;
    blocs->brick_start.w *= 2;
    blocs->brick_start.h *= 2;

    /* Selection rectangle brick start */
    blocs->select_start = blocs->brick_start;
    blocs->select_start.w += 4 ;
    blocs->select_start.x -= 2;
    blocs->select_start.h += 4;
    blocs->select_start.y -= 2;

    /* Brick exit */
    blocs->brick_exit.x -= blocs->brick_exit.w/2;
    blocs->brick_exit.w = blocs->brick_start.w;
    blocs->brick_exit.h = blocs->brick_start.h;
    blocs->brick_exit.y = blocs->brick_start.y;

    /* Selection rectangle brick exit */
    blocs->select_exit = blocs->brick_exit;
    blocs->select_exit.w += 4;
    blocs->select_exit.x -= 2;
    blocs->select_exit.h += 4;
    blocs->select_exit.y -= 2;

    blocs->texture = creerTextureImg(ecran,"Images/bloc_menu.bmp",NULL,NULL);
}

/* Initialization of the endgame images (win/lose) */
void Init_end_game(SDL_Renderer* ecran, End_game* fin_jeu){
    fin_jeu->win_texture = creerTextureImg(ecran,"Images/you_win.bmp",NULL,NULL);
    fin_jeu->lose_texture = creerTextureImg(ecran,"Images/game_over.bmp",NULL,NULL);

    fin_jeu->end_rect.w = SCREEN_WIDTH/2;
    fin_jeu->end_rect.h = SCREEN_HEIGHT/2;
    fin_jeu->end_rect.x = SCREEN_WIDTH/4;
    fin_jeu->end_rect.y = SCREEN_HEIGHT/4;

    fin_jeu->lose = FALSE;
    fin_jeu->win = FALSE;
}

/* Draw a rectangle */
void dessine_rect_plein(SDL_Renderer *gRenderer,SDL_Rect *rect,long c){
   int r = ( uint8_t )(c >> 16);
   int g = ( uint8_t )(c >>  8);
   int b = (  uint8_t  )c;
   SDL_SetRenderDrawColor( gRenderer,r,g,b,255);
   SDL_RenderFillRect( gRenderer,rect);
}

/* Get the current time in ms */
double getCurrentTimeMS(){
    double clo=clock();
    return 1000.*clo/CLOCKS_PER_SEC;
}

/* Create the textures */
SDL_Texture* creerTextureImg(SDL_Renderer *ecran,char *nom_file_bmp,int *size_w, int *size_h){
    SDL_Surface* image = SDL_LoadBMP(nom_file_bmp);
    if(!image)
    {
      printf("Loading image error : %s",SDL_GetError());
      return NULL;
    }

    SDL_Texture* monImage = SDL_CreateTextureFromSurface(ecran,image);
    SDL_FreeSurface(image);

    if(NULL == monImage)
    {
        fprintf(stderr, "Error SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    SDL_QueryTexture(monImage, NULL, NULL,size_w,size_h); 


    return monImage;
}

/* Display the image (texture) on screen */
void dessine_Img_redim(SDL_Renderer *ecran,SDL_Texture *imgTexture,SDL_Rect imgRect){
    SDL_Rect pos_texture; // taille originelle
    pos_texture.x=0;
    pos_texture.y=0;
    SDL_QueryTexture(imgTexture,NULL,NULL,&pos_texture.w,&pos_texture.h);// recupere la taille de l'image originelle
    SDL_RenderCopy(ecran, imgTexture,&pos_texture, &imgRect);
}

/* Initialization of the variables for the images (background/win/lose images) */
void Init_image(SDL_Renderer* ecran, SDL_Texture** image_Texture, SDL_Rect* img_rect, char* file_img){
    *image_Texture=creerTextureImg(ecran,file_img,&img_rect->w,&img_rect->h);
    
    /* Display of the background (4 images to have a smooth effect) */
    if(strcmp(file_img,"Images/fond_super_mario_1.bmp") == 0){
        img_rect->w = SCREEN_WIDTH/2;
        img_rect->h = SCREEN_HEIGHT;
        img_rect->x = 0;
        img_rect->y = 0;
    }

    else if(strcmp(file_img,"Images/fond_super_mario_2.bmp") == 0){
        img_rect->w = SCREEN_WIDTH/2;
        img_rect->h = SCREEN_HEIGHT;
        img_rect->x = SCREEN_WIDTH/2;
        img_rect->y = 0;
    }

    else if(strcmp(file_img,"Images/fond_super_mario_3.bmp") == 0){
        img_rect->w = SCREEN_WIDTH/2;
        img_rect->h = SCREEN_HEIGHT;
        img_rect->x = SCREEN_WIDTH;
        img_rect->y = 0;
    }

    else if(strcmp(file_img,"Images/fond_super_mario_4.bmp") == 0){
        img_rect->w = SCREEN_WIDTH/2;
        img_rect->h = SCREEN_HEIGHT;
        img_rect->x = 3*SCREEN_WIDTH/2;
        img_rect->y = 0;
    }

    /* Display if the game is won */
    else if(strcmp(file_img,"Images/you_win_bmp.bmp") == 0){
        img_rect->w=img_rect->w/2 + SCREEN_WIDTH - 640;
        img_rect->h=img_rect->h/2 + SCREEN_HEIGHT - 480;
        img_rect->x=SCREEN_WIDTH/2 - img_rect->w/2;
        img_rect->y=SCREEN_HEIGHT/2 - img_rect->h/2;
    }

    /* Display if the game is lost */
    else if(strcmp(file_img,"Images/game_over_bmp.bmp") == 0){
        img_rect->w=img_rect->w/4 + SCREEN_WIDTH - 640;
        img_rect->h=img_rect->h/4 + SCREEN_HEIGHT - 480;
        img_rect->x=SCREEN_WIDTH/2 - img_rect->w/2;
        img_rect->y=SCREEN_HEIGHT/2 - img_rect->h/2;
    }
}

/* Initialization of the variables for the text (game menu) */
void Init_police(SDL_Renderer* ecran, char* message, SDL_Texture** texture, char* mode, SDL_Rect* pos_rect){
    if (*texture) {SDL_DestroyTexture(*texture);*texture=NULL;}
    SDL_Color Or = {238, 201, 0};
    SDL_Color Or_2 = {139, 117,   0};
    TTF_Font* font = NULL;
    SDL_Surface* surface = NULL;

    if(strcmp(mode,"start") == 0){
        font = TTF_OpenFont( "EivenMajor.ttf", 100);
        surface = TTF_RenderText_Solid(font,message,Or);
        *texture = SDL_CreateTextureFromSurface(ecran,surface);
        
        /* Position of "start" */
        pos_rect->w = SCREEN_WIDTH/9;
        pos_rect->h = SCREEN_HEIGHT/12; 
        pos_rect->x = 5*SCREEN_WIDTH/16;
        pos_rect->y = 3*SCREEN_HEIGHT/4 - pos_rect->h;
    }

    else if(strcmp(mode,"exit") == 0){
        font = TTF_OpenFont("EivenMajor.ttf",100);
        surface = TTF_RenderText_Solid(font,message,Or);
        *texture = SDL_CreateTextureFromSurface(ecran,surface);
        
        /* Position of "exit" */
        pos_rect->w = SCREEN_WIDTH/9;
        pos_rect->h = SCREEN_HEIGHT/12; 
        pos_rect->x = 9*SCREEN_WIDTH/16;
        pos_rect->y = 3*SCREEN_HEIGHT/4 - pos_rect->h;
    }

    else if(strcmp(mode,"game_name") == 0){
        font = TTF_OpenFont("EivenMajor.ttf",200);
        surface = TTF_RenderText_Solid(font,message,Or);
        *texture = SDL_CreateTextureFromSurface(ecran,surface);

        /* Position of the name of the game */
        pos_rect->w = SCREEN_WIDTH/2;
        pos_rect->h = SCREEN_HEIGHT/4;
        pos_rect->x = SCREEN_WIDTH/4;
        pos_rect->y = SCREEN_HEIGHT/4;
    }

    else if(strcmp(mode,"game_name_2") == 0){
        font = TTF_OpenFont("EivenMajor.ttf",200);
        surface = TTF_RenderText_Solid(font,message,Or_2);  
        *texture = SDL_CreateTextureFromSurface(ecran,surface);

        /*Position of the name of the game (twice for 3D effect) */
        pos_rect->w = SCREEN_WIDTH/2;
        pos_rect->h = SCREEN_HEIGHT/4;
        pos_rect->x = SCREEN_WIDTH/4 + 4;
        pos_rect->y = SCREEN_HEIGHT/4;
    }

    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
}

/*Initialization of the music and sound effects */
void Init_musique(Effets_sonores* musique){

    /* Loading of the musics and sound effects */
    musique->select = Mix_LoadWAV("Musique/select_sound.mp3");
    musique->menu = Mix_LoadWAV("Musique/Menu.mp3");
    musique->jeu = Mix_LoadWAV("Musique/JEU.mp3");
    musique->marche = Mix_LoadWAV("Musique/MARCHE.mp3");
    musique->saut = Mix_LoadWAV("Musique/SAUT.mp3");
    musique->oiseau = Mix_LoadWAV("Musique/oiseau.mp3");
    musique->arraigne = Mix_LoadWAV("Musique/ARRAIGNE.mp3");
    musique->WIN_SOUND_EFFECT = Mix_LoadWAV("Musique/GAME_WIN_SOUND_EFFECT.mp3");
    musique->GAME_OVER_SOUND_EFFECT = Mix_LoadWAV("Musique/GAME_OVER_SOUND_EFFECT.mp3");
    musique->Here_we_go = Mix_LoadWAV("Musique/here_we_go.mp3");
}

/* Free the memory allocated for the musics and sound effects*/
void free_audio(Effets_sonores* Musique){
    if(Musique){
        if (Musique->select) {Mix_FreeChunk(Musique->select); Musique->select = NULL;}
        if (Musique->jeu) {Mix_FreeChunk(Musique->jeu); Musique->jeu = NULL;}
        if (Musique->menu) {Mix_FreeChunk(Musique->menu); Musique->menu = NULL;}
        if (Musique->saut) {Mix_FreeChunk(Musique->saut); Musique->saut = NULL;}
        if (Musique->marche) {Mix_FreeChunk(Musique->marche); Musique->marche = NULL;}
        if (Musique->oiseau) {Mix_FreeChunk(Musique->oiseau); Musique->oiseau = NULL;}
        if (Musique->arraigne) {Mix_FreeChunk(Musique->arraigne); Musique->arraigne = NULL;}
        if (Musique->WIN_SOUND_EFFECT) {Mix_FreeChunk(Musique->WIN_SOUND_EFFECT); Musique->WIN_SOUND_EFFECT = NULL;}
        if (Musique->GAME_OVER_SOUND_EFFECT) {Mix_FreeChunk(Musique->GAME_OVER_SOUND_EFFECT); Musique->GAME_OVER_SOUND_EFFECT = NULL;}
        if (Musique->Here_we_go) {Mix_FreeChunk(Musique->Here_we_go); Musique->Here_we_go = NULL;}
    }
}

/* Initialization of the cave (end scene) */
void Init_cave(SDL_Renderer* ecran, Caverne* cave){
    /* The cave is cut in half to allow the character to enter it (effect) */
    cave->texture_partie_droite = creerTextureImg(ecran,"Images/caverne_droite.bmp",&cave->rect_d.w,&cave->rect_d.h);
    cave->texture_partie_gauche = creerTextureImg(ecran,"Images/caverne_gauche.bmp",&cave->rect_g.w,&cave->rect_g.h);

    /* Position of the left part */
    cave->rect_g.x = SCREEN_WIDTH;
    cave->rect_g.y = SCREEN_HEIGHT - cave->rect_g.h + 3;

    /* Position of the right part */
    cave->rect_d.x = cave->rect_g.x + cave->rect_g.w - 100; 
    cave->rect_d.y = SCREEN_HEIGHT - cave->rect_d.h + 3;

    cave->cpt_pixel = 0;
    cave->pixel_target = 7*SCREEN_WIDTH;
    cave->move = FALSE;
    cave->on_screen = FALSE;
}

/* Manage the movement of the background */
void Deplacement_fond(Arriere_plan* decor, int vitesse){

    decor->fond_1.x -= vitesse;
    decor->fond_2.x -= vitesse;
    decor->fond_3.x -= vitesse;
    decor->fond_4.x -= vitesse;
    
    if((decor->fond_1.x + decor->fond_1.w) < 0){
        decor->fond_1.x = decor->fond_4.x + decor->fond_4.w;
    }
    else if((decor->fond_2.x + decor->fond_2.w) < 0){
        decor->fond_2.x = decor->fond_1.x + decor->fond_1.w;
    }
    else if((decor->fond_3.x + decor->fond_3.w) < 0){
        decor->fond_3.x = decor->fond_2.x + decor->fond_2.w;
    }
    else if((decor->fond_4.x + decor->fond_4.w) < 0){
        decor->fond_4.x = decor->fond_3.x + decor->fond_3.w;
    }

}

/* Display the background */
void dessine_fond(SDL_Renderer* ecran, Arriere_plan decor){
    dessine_Img_redim(ecran,decor.texture_1,decor.fond_1);
    dessine_Img_redim(ecran,decor.texture_2,decor.fond_2);
    dessine_Img_redim(ecran,decor.texture_3,decor.fond_3);
    dessine_Img_redim(ecran,decor.texture_4,decor.fond_4);
}

/* Destroy the textures */
void free_decor(Arriere_plan* decor){
    if(decor){
        if(decor->texture_1){SDL_DestroyTexture(decor->texture_1); decor->texture_1 = NULL;}
        if(decor->texture_2){SDL_DestroyTexture(decor->texture_2); decor->texture_2 = NULL;}
        if(decor->texture_3){SDL_DestroyTexture(decor->texture_3); decor->texture_3 = NULL;}
        if(decor->texture_4){SDL_DestroyTexture(decor->texture_4); decor->texture_4 = NULL;}
    }
}
