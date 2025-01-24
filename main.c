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

   /* Initialisation de SDL_Mixer */
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    /* Initialisation de SDL_TTF */
    if(TTF_Init() < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_TTF : %s", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    /* Initialisation des effets sonores */
    Effets_sonores musique;
    Init_musique(&musique);
    
    if( !success ){
         fprintf(stderr,"Erreur de creation de la fenetre: %s\n",SDL_GetError());
    }
    
    else{
	    /* ------ INITIALISATION ------ */

        /* Allocation des canaux */
        Mix_AllocateChannels(3);

        /* Reglages du volume des canaux */
        Mix_Volume(0,128);     // Canal musique jeu/menu/fin de jeu
        Mix_Volume(1,110);     // Canal marche et saut personnage
        Mix_Volume(2,110);     // Canal bruit sonores(ennemis/personnage)


        /* Initialisation de l'arriere plan (defilement) */
        Arriere_plan decor;
        Init_image(ecran,&decor.texture_1,&decor.fond_1,"Images/fond_super_mario_1.bmp");
        Init_image(ecran,&decor.texture_2,&decor.fond_2,"Images/fond_super_mario_2.bmp");
        Init_image(ecran,&decor.texture_3,&decor.fond_3,"Images/fond_super_mario_3.bmp");
        Init_image(ecran,&decor.texture_4,&decor.fond_4,"Images/fond_super_mario_4.bmp");
        
        /* Initialisation du texte */
        SDL_Rect name_rect, start_rect, exit_rect, name_rect_2;
        char game_str[14] = "Jungle Bros.", start_str[8] = " Start ",exit_str[7] = " Exit ";
        Init_police(ecran,game_str,&name_texture,"game_name",&name_rect);
        Init_police(ecran,start_str,&start_texture,"start",&start_rect);
        Init_police(ecran,exit_str,&exit_texture,"exit",&exit_rect);
        Init_police(ecran,game_str,&name_texture_2,"game_name_2",&name_rect_2);

        /* Initialisation des blocs (menu) */
        Brick_menu blocs;
        Init_blocs(ecran,start_rect,exit_rect,&blocs);

        /* Initialisation des images de fin de jeu (lose/win) */
        End_game fin_jeu;
        Init_end_game(ecran,&fin_jeu);
        
        /* Initialisation du personnage */
        Personnage mario;
        Init_personnage(ecran, &mario);

        /* Initialisation de l'obstacle */
        Obstacle obs;
        Init_obstacle(ecran,&obs);

        /* Initialisation de la caverne (fin) */
        Caverne cave;
        Init_cave(ecran, &cave);

        /* Initialisation ennemis */
        Ennemi* ennemi[MAX_ENNEMIS];
        ennemi[0] = initEnnemi(ecran, TYPE_OISEAU);  // Initialise l'oiseau avec les paramètres appropriés
        ennemi[1] = initEnnemi(ecran, TYPE_ARRAIGNE); // Initialise l'arraigné avec les paramètres appropriés

        /* Initialisation des variables */
        int quit = FALSE, menu = TRUE;
        int wait = 0;
        int compt_endgame = 0;

        /* Controle du temps pour les mises à jour */
        double previous = getCurrentTimeMS();
        double lag = 0.0;
        double current = 0.0;
    

        /* Lancement de la musique du menu */
        Mix_PlayChannel(0,musique.menu, -1);

        /* BOUCLE DU JEU / GAME LOOP */
        while( !quit ){
            current = getCurrentTimeMS();
            elapsed = current - previous;
            previous = current;
            lag += elapsed;
            moveDistance_walk = (int) SPEED_WALK * (elapsed/1000.0);
            
            if(!fin_jeu.win && !fin_jeu.lose && !menu && !mario.exit_menu && !mario.exit_game){

                /* Deplacement personnage */
                control_keybord(ecran,&quit,&mario,&fin_jeu,musique,&obs, &cave, &decor, ennemi);
                
                /* Deplacement ennemis */
                updateEnnemi(ennemi,elapsed,&musique,cave,obs); 
                

                for (int i = 0; i < MAX_ENNEMIS; i++) {
                    updateAnimation(ennemi[i]);

                    /* Si le personnage et l'ennemi sont toujours en vie */
                    if (ennemi[i]->isAlive && mario.isAlive) { 
                        
                        /* Le personnage meurt en cas de collision */
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

                /* ------ MISE A JOUR ------ */
                
                lag -= MS_PER_UPDATE;
                
                if(menu || mario.exit_menu){
                    Mix_Volume(0,128);
                    mario.alternance = FALSE;
                    menu_control(ecran,&blocs,&quit,&menu,musique);

                    /* Clignotement du bouton selectionné */
                    if(blocs.selection_exit || blocs.selection_start)  blocs.clignotement = !blocs.clignotement; 
                    
                    if(!menu && !quit && !mario.exit_menu){ /*  <=> Appui Start */
                        mario.exit_menu = TRUE;
                        mario.dir = 'd';

                        /* Lancement du son "ah ah" du personnage*/
                        Mix_PlayChannel(2, musique.Here_we_go, 0);

                        /* Arret de la musique du menu et lancement de celle du jeu */
                        Mix_HaltChannel(0);
                        Mix_PlayChannel(0,musique.jeu, -1);
                    }

                    /* Animation : le personnage sort du menu */
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
                    /* ------ GESTION DE LA PARTIE ------ */
                    Mix_Volume(0,110);

                    /* Animation : le personnage rentre dans la grotte (fin de partie) */
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

                        /* Reinitialisation des elements du jeu */
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

                            /* Oiseau */
                            ennemi[0]->x = SCREEN_WIDTH + 200;
                            ennemi[0]->y = SCREEN_HEIGHT/2 - ennemi[0]->rect.h/2;
                            ennemi[0]->isAlive = FALSE;
                            ennemi[0]->speed_start = 120;
                            ennemi[0]->speed = ennemi[0]->speed_start;
                            
                            /* Arraigne */
                            ennemi[1]->x = SCREEN_WIDTH + 200 - ennemi[1]->rect.w/2;
                            ennemi[1]->y = COORD_SOL;
                            ennemi[1]->isAlive = FALSE;
                            ennemi[1]->speed_start = 100;
                            ennemi[1]->speed = ennemi[1]->speed_start;
                            
                            /* Reprise de l'image personnage vivant */
                            if(fin_jeu.lose){
                                int rect_tmp;
                                rect_tmp =  mario.rect.w;
                                mario.rect.w = mario.rect.h;
                                mario.rect.h = rect_tmp;
                                mario.x = 3*SCREEN_WIDTH/4 + mario.rect.w/2;
                            }
                            
                            fin_jeu.win = FALSE;
                            fin_jeu.lose = FALSE;
                            
                            /* Arret des musiques et lancement de celle du menu */
                            Mix_HaltChannel(-1);
                            Mix_PlayChannel(0,musique.menu, -1);
                        }
                    }

                    
                    /* Effet sonore 'marche' non actif quand le personnage saute */
                    if(!fin_jeu.lose && !fin_jeu.win && (mario.y == mario.previous_y) && ((mario.dir == 'g') || (mario.dir == 'd'))){ 
                        if(!Mix_Playing(1))
                            Mix_PlayChannel(1,musique.marche, -1);
                    }

                    if(wait%2) mario.alternance = !mario.alternance;
                    wait++;
                }

                lag -= MS_PER_UPDATE;
    
            }

            /* ------ AFFICHAGE ------ */

            SDL_SetRenderDrawColor( ecran, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear(ecran);

            /* dessine l'image de fond */
            dessine_fond(ecran, decor);

            /* Affichage du menu d'acceuil */
            if(menu){
                if(blocs.clignotement){
                    if(blocs.selection_exit) dessine_rect_plein(ecran,&blocs.select_exit,White);
                    else if(blocs.selection_start){ dessine_rect_plein(ecran,&blocs.select_start,White); }
                }

                /* Affichage des briques */
                dessine_Img_redim(ecran,blocs.texture,blocs.brick_exit);
                dessine_Img_redim(ecran,blocs.texture,blocs.brick_start);
                
                /* Affichage du texte */
                SDL_RenderCopy(ecran,name_texture_2,NULL,&name_rect_2);  
                SDL_RenderCopy(ecran,name_texture,NULL,&name_rect);
                SDL_RenderCopy(ecran,start_texture,NULL,&start_rect);
                SDL_RenderCopy(ecran,exit_texture,NULL,&exit_rect);
            }

            dessine_Img_redim(ecran,cave.texture_partie_gauche,cave.rect_g);        /* Caverne partie gauche*/
            dessine_personnage(ecran,&mario);                                       /* Personnage */
            dessine_Img_redim(ecran,cave.texture_partie_droite,cave.rect_d);        /* Caverne partie droite*/
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

        /* Desallocation mémoire */
        free_mario(&mario);
        for(int n = 0; n < MAX_ENNEMIS; n++)
            free_ennemis(&ennemi[n]);
        
        
        /* Destruction des textures */
        // Bouton de selection (menu)
        if(blocs.texture){SDL_DestroyTexture(blocs.texture); blocs.texture = NULL;}
        
        // Obstacle
        if(obs.texture){SDL_DestroyTexture(obs.texture); obs.texture = NULL;}
        
        // Image de fin de jeu (win/game over)
        if(fin_jeu.win_texture){SDL_DestroyTexture(fin_jeu.win_texture); fin_jeu.win_texture = NULL;}
        if(fin_jeu.lose_texture){SDL_DestroyTexture(fin_jeu.lose_texture); fin_jeu.lose_texture = NULL;}
        
        // Caverne (fin de jeu)
        if(cave.texture_partie_droite){SDL_DestroyTexture(cave.texture_partie_droite); cave.texture_partie_droite = NULL;}
        if(cave.texture_partie_gauche){SDL_DestroyTexture(cave.texture_partie_gauche); cave.texture_partie_gauche = NULL;}
        
        // Arriere plan
        free_decor(&decor);
	}

    /* Destruction des textures */
    //Texte
    if(name_texture){SDL_DestroyTexture(name_texture); name_texture = NULL;}
    if(start_texture){SDL_DestroyTexture(start_texture); start_texture = NULL;}
    if(exit_texture){SDL_DestroyTexture(exit_texture); exit_texture = NULL;}

    if(name_texture_2){SDL_DestroyTexture(name_texture_2); name_texture_2 = NULL;}      // TEST : A enlever

    TTF_Quit();

    /* Libère les musiques et effets sonores */
    Mix_HaltChannel(-1);  
    free_audio(&musique);
    Mix_CloseAudio();

    /* Fermeture de l'ecran et de la fenetre */
    close(&fenetre,&ecran);
    return 0;
}

