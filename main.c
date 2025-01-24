#include "game_settings.h"
#include "mario.h"
#include "ennemis.h"

double elapsed;
int moveDistance_walk;

int main(int argc, char** argv)
{
   int success=FALSE;
   SDL_Window* fenetre=NULL;
   SDL_Renderer* ecran=NULL;
   SDL_Texture* name_texture = NULL;
   SDL_Texture* start_texture = NULL;
   SDL_Texture* exit_texture = NULL;
   SDL_Texture* name_texture_2 = NULL;


   success=init(&fenetre,&ecran,"Jungle Bros.");

   /* Initialization of SDL_Mixer */
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    /* Initialization of SDL_TTF */
    if(TTF_Init() < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_TTF : %s", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    /* Initialization of the sound effects */
    Effets_sonores musique;
    Init_musique(&musique);
    
    if( !success ){
         fprintf(stderr,"Erreur de creation de la fenetre: %s\n",SDL_GetError());
    }
    
    else{
	/* ------ INITIALIZATION ------ */

        /* Allocation of the sound channels */
        Mix_AllocateChannels(3);

        /* Channels volume control */
        Mix_Volume(0,128);     // Channel for the game/menu/end game music
        Mix_Volume(1,110);     // Channel for the walking and jumping sounds (character)
        Mix_Volume(2,110);     // Channels for other sound effects (enemies/character)


        /*Initialization of the background */
        Arriere_plan decor;
        Init_image(ecran,&decor.texture_1,&decor.fond_1,"Images/fond_super_mario_1.bmp");
        Init_image(ecran,&decor.texture_2,&decor.fond_2,"Images/fond_super_mario_2.bmp");
        Init_image(ecran,&decor.texture_3,&decor.fond_3,"Images/fond_super_mario_3.bmp");
        Init_image(ecran,&decor.texture_4,&decor.fond_4,"Images/fond_super_mario_4.bmp");
        
        /* Initialization of the text */
        SDL_Rect name_rect, start_rect, exit_rect, name_rect_2;
        char game_str[14] = "Jungle Bros.", start_str[8] = " Start ",exit_str[7] = " Exit ";
        Init_police(ecran,game_str,&name_texture,"game_name",&name_rect);
        Init_police(ecran,start_str,&start_texture,"start",&start_rect);
        Init_police(ecran,exit_str,&exit_texture,"exit",&exit_rect);
        Init_police(ecran,game_str,&name_texture_2,"game_name_2",&name_rect_2);

        /* Initialization of the bricks (menu) */
        Brick_menu blocs;
        Init_blocs(ecran,start_rect,exit_rect,&blocs);

        /* Initialization of the end game images (lose/win) */
        End_game fin_jeu;
        Init_end_game(ecran,&fin_jeu);
        
        /* Initialization of the character */
        Personnage mario;
        Init_personnage(ecran, &mario);

        /* Initialization of the obstacle */
        Obstacle obs;
        Init_obstacle(ecran,&obs);

        /* Initialization of the cave (end) */
        Caverne cave;
        Init_cave(ecran, &cave);

        /* Initialization of the enemies */
        Ennemi* ennemi[MAX_ENNEMIS];
        ennemi[0] = initEnnemi(ecran, TYPE_OISEAU);  // BIRD
        ennemi[1] = initEnnemi(ecran, TYPE_ARRAIGNE); // SPIDER

        /* Initialization of the game variables */
        int quit = FALSE, menu = TRUE;
        int wait = 0;
        int compt_endgame = 0;

        /* Time control for the updates */
        double previous = getCurrentTimeMS();
        double lag = 0.0;
        double current = 0.0;
    

        /* Start of the game menu music */
        Mix_PlayChannel(0,musique.menu, -1);

        /* GAME LOOP */
        while( !quit ){
            current = getCurrentTimeMS();
            elapsed = current - previous;
            previous = current;
            lag += elapsed;
            moveDistance_walk = (int) SPEED_WALK * (elapsed/1000.0);
            
            if(!fin_jeu.win && !fin_jeu.lose && !menu && !mario.exit_menu && !mario.exit_game){

                /* Movement of the character */
                control_keybord(ecran,&quit,&mario,&fin_jeu,musique,&obs, &cave, &decor, ennemi);
                
                /* Movement of the ennemies */
                updateEnnemi(ennemi,elapsed,&musique,cave,obs); 
                

                for (int i = 0; i < MAX_ENNEMIS; i++) {
                    updateAnimation(ennemi[i]);

                    /* If the character and the enemy are still alive */
                    if (ennemi[i]->isAlive && mario.isAlive) { 
                        
                        /* The character dies if he touches an enemy */
                        mario.rect.x = mario.x - mario.rect.w/2;
                        mario.rect.y = mario.y - mario.rect.h/2;
                        ennemi[i]->rect.x = ennemi[i]->x - ennemi[i]->rect.w/2;
                        ennemi[i]->rect.y = ennemi[i]->y - ennemi[i]->rect.h/2; 
                        if (checkCollision(mario.rect, ennemi[i]->rect)) {
                            mario.isAlive = 0; 
                            Mix_PlayChannel(0,musique.GAME_OVER_SOUND_EFFECT, 0);
                            fin_jeu.lose = TRUE;  
                        }
                    }
                
                }
            }
            
            if(mario.exit_menu) mario.x += moveDistance_walk;
            else if(mario.exit_game) mario.x += moveDistance_walk;
            
            while (lag >= MS_PER_UPDATE){

                /* ------ UPDATE ------ */
                
                lag -= MS_PER_UPDATE;
                
                if(menu || mario.exit_menu){
                    Mix_Volume(0,128);
                    mario.alternance = FALSE;
                    menu_control(ecran,&blocs,&quit,&menu,musique);

                    /* Blinking of the selected button */
                    if(blocs.selection_exit || blocs.selection_start)  blocs.clignotement = !blocs.clignotement; 
                    
                    if(!menu && !quit && !mario.exit_menu){ /*  <=> Start button selected */
                        mario.exit_menu = TRUE;
                        mario.dir = 'd';

                        /* Playing of the "ah ah" character sound */
                        Mix_PlayChannel(2, musique.Here_we_go, 0);

                        /* Stop of the menu music and playing the game music */
                        Mix_HaltChannel(0);
                        Mix_PlayChannel(0,musique.jeu, -1);
                    }

                    /* Animation : the character exits the screen */
                    if(mario.exit_menu){
                        if(wait%2) mario.alternance = !mario.alternance;
                        wait++;
                        if(mario.x >= (SCREEN_WIDTH + mario.rect.w/2)){
                            mario.exit_menu = FALSE;
                            mario.x = mario.rect.w/2;
                            decor.fond_3.x = 0;
                            decor.fond_4.x = SCREEN_WIDTH/2;
                            decor.fond_1.x = SCREEN_WIDTH;
                            decor.fond_2.x = 3*SCREEN_WIDTH/2;
                            wait = 0;
                        }
                        if(!Mix_Playing(1))
                            Mix_PlayChannel(1,musique.marche, -1);
                    }
                } 

                else{
                    /* ------ GAME CONTROL ------ */
                    Mix_Volume(0,110);

                    /* Animation : the character enters the cave (end game) */
                    if(mario.exit_game){
                        mario.y = mario.previous_y;
                        if((mario.x - mario.rect.w/2) >= (cave.rect_d.x + cave.rect_d.w/2)){
                            mario.exit_game = FALSE;
                            fin_jeu.win = TRUE;
                        }
                    }

                    if(fin_jeu.win || fin_jeu.lose){
                        compt_endgame++;
                        
                        if(Mix_Playing(1)) Mix_HaltChannel(1);
                        if(Mix_Playing(2)) Mix_HaltChannel(2);

                        /* Reset of all the variables  */
                        if(((compt_endgame == 50) && fin_jeu.lose) || ((compt_endgame == 30) && fin_jeu.win)){
                            menu = TRUE;
                            compt_endgame = 0;
                            mario.previous_dir = 'd';
                            mario.dir = ' ';
                            mario.x = 3*SCREEN_WIDTH/4 + mario.rect.w/2;              
                            mario.y = mario.previous_y;                               
                            mario.target_y = mario.previous_y;
                            mario.down = FALSE;
                            mario.up = FALSE;
                            obs.x = SCREEN_WIDTH + obs.rect.w/2;                      
                            cave.rect_g.x = SCREEN_WIDTH;                             
                            cave.rect_d.x = cave.rect_g.x + cave.rect_g.w - 100;      
                            cave.move = FALSE;
                            cave.on_screen = FALSE;
                            cave.cpt_pixel = 0;
                            decor.fond_1.x = 0;
                            decor.fond_2.x = SCREEN_WIDTH/2;
                            decor.fond_3.x = SCREEN_WIDTH;
                            decor.fond_4.x = 3*SCREEN_WIDTH/2;
                            mario.isAlive = TRUE;

                            /* Bird */
                            ennemi[0]->x = SCREEN_WIDTH + 200;
                            ennemi[0]->y = SCREEN_HEIGHT/2 - ennemi[0]->rect.h/2;
                            ennemi[0]->isAlive = FALSE;
                            ennemi[0]->speed_start = 120;
                            ennemi[0]->speed = ennemi[0]->speed_start;
                            
                            /* Spider */
                            ennemi[1]->x = SCREEN_WIDTH + 200 - ennemi[1]->rect.w/2;
                            ennemi[1]->y = COORD_SOL;
                            ennemi[1]->isAlive = FALSE;
                            ennemi[1]->speed_start = 100;
                            ennemi[1]->speed = ennemi[1]->speed_start;
                            
                            /* Reset of the image parameters of the character (dead --> alive) */
                            if(fin_jeu.lose){
                                int rect_tmp;
                                rect_tmp =  mario.rect.w;
                                mario.rect.w = mario.rect.h;
                                mario.rect.h = rect_tmp;
                                mario.x = 3*SCREEN_WIDTH/4 + mario.rect.w/2;
                            }
                            
                            fin_jeu.win = FALSE;
                            fin_jeu.lose = FALSE;
                            
                            /* Stoping all musics and sound effects and playing the game menu music*/
                            Mix_HaltChannel(-1);
                            Mix_PlayChannel(0,musique.menu, -1);
                        }
                    }

                    
                    /* When the character jumps, the walking sound effect must not be played */
                    if(!fin_jeu.lose && !fin_jeu.win && (mario.y == mario.previous_y) && ((mario.dir == 'g') || (mario.dir == 'd'))){ 
                        if(!Mix_Playing(1))
                            Mix_PlayChannel(1,musique.marche, -1);
                    }

                    if(wait%2) mario.alternance = !mario.alternance;
                    wait++;
                }

                lag -= MS_PER_UPDATE;
    
            }

            /* ------ DISPLAY ------ */

            SDL_SetRenderDrawColor( ecran, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear(ecran);

            /* Display of the background */
            dessine_fond(ecran, decor);

            /* Display of the game menu */
            if(menu){
                if(blocs.clignotement){
                    if(blocs.selection_exit) dessine_rect_plein(ecran,&blocs.select_exit,White);
                    else if(blocs.selection_start){ dessine_rect_plein(ecran,&blocs.select_start,White); }
                }

                /* Display of the bricks */
                dessine_Img_redim(ecran,blocs.texture,blocs.brick_exit);
                dessine_Img_redim(ecran,blocs.texture,blocs.brick_start);
                
                /* Display of the text */
                SDL_RenderCopy(ecran,name_texture_2,NULL,&name_rect_2);  
                SDL_RenderCopy(ecran,name_texture,NULL,&name_rect);
                SDL_RenderCopy(ecran,start_texture,NULL,&start_rect);
                SDL_RenderCopy(ecran,exit_texture,NULL,&exit_rect);
            }

            dessine_Img_redim(ecran,cave.texture_partie_gauche,cave.rect_g);        /* Left part CAVE */
            dessine_personnage(ecran,&mario);                                       /* Character */
            dessine_Img_redim(ecran,cave.texture_partie_droite,cave.rect_d);        /* Right part CAVE*/
            dessine_obstacle(ecran,obs);                                            /* Obstacle */
            
            for (int i = 0; i < MAX_ENNEMIS; i++) {
                dessine_Ennemis(ennemi[i], ecran); 
            }

            if(fin_jeu.win){
                dessine_Img_redim(ecran,fin_jeu.win_texture,fin_jeu.end_rect);
            }
            else if(fin_jeu.lose){
                dessine_Img_redim(ecran,fin_jeu.lose_texture,fin_jeu.end_rect);
            }


            SDL_RenderPresent(ecran);
        }

        /* Free the memory allocated */
        free_mario(&mario);
        for(int n = 0; n < MAX_ENNEMIS; n++)
            free_ennemis(&ennemi[n]);
        
        
        /* Destruction of the textures */
        // Selection button (menu)
        if(blocs.texture){SDL_DestroyTexture(blocs.texture); blocs.texture = NULL;}
        
        // Obstacle
        if(obs.texture){SDL_DestroyTexture(obs.texture); obs.texture = NULL;}
        
        // End game image (win/game over)
        if(fin_jeu.win_texture){SDL_DestroyTexture(fin_jeu.win_texture); fin_jeu.win_texture = NULL;}
        if(fin_jeu.lose_texture){SDL_DestroyTexture(fin_jeu.lose_texture); fin_jeu.lose_texture = NULL;}
        
        // Cave (end game)
        if(cave.texture_partie_droite){SDL_DestroyTexture(cave.texture_partie_droite); cave.texture_partie_droite = NULL;}
        if(cave.texture_partie_gauche){SDL_DestroyTexture(cave.texture_partie_gauche); cave.texture_partie_gauche = NULL;}
        
        //Background
        free_decor(&decor);
	}

    /* Destruction of the textures */
    //Text
    if(name_texture){SDL_DestroyTexture(name_texture); name_texture = NULL;}
    if(start_texture){SDL_DestroyTexture(start_texture); start_texture = NULL;}
    if(exit_texture){SDL_DestroyTexture(exit_texture); exit_texture = NULL;}

    if(name_texture_2){SDL_DestroyTexture(name_texture_2); name_texture_2 = NULL;}      // TEST : A enlever

    TTF_Quit();

    /* Free the memory space allocated for musics and sound effects */
    Mix_HaltChannel(-1);  
    free_audio(&musique);
    Mix_CloseAudio();

    /* Close the screen and window */
    close(&fenetre,&ecran);
    return 0;
}

