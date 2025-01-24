#include "mario.h"

extern double elapsed;
extern int moveDistance_walk;

/* Manage the interaction between the player and the keybord */
void control_keybord( SDL_Renderer* ecran, int* quit, Personnage* mario, End_game* fin_jeu, Effets_sonores musique, Obstacle* bloc, Caverne* cave, Arriere_plan* decor, Ennemi* ennemis[MAX_ENNEMIS]){
    SDL_Event e; 
    const Uint8* touches;
    int touche_up_relache = FALSE;
    int moveDistance_jump = (int) SPEED_JUMP * (elapsed/1000.0);

    touches = SDL_GetKeyboardState(NULL);

    if(!touches[SDL_SCANCODE_SPACE]) touche_up_relache = TRUE;
    else{ touche_up_relache = FALSE; }

    /* Rise (jump) */
    if(mario->up){
        if(mario->y == mario->target_y){
            
            /* Static jump (up and down) */
            if((mario->dir == ' ') && (!mario->tps_up)){
                mario->down = TRUE;
                mario->up = FALSE;
                mario->target_y = mario->previous_y;
                mario->tps_up = 0;
            }
            mario->tps_up++;

            /* Jump */
            if(mario->tps_up == 5){
                mario->down = TRUE;
                mario->up = FALSE;
                mario->target_y = mario->previous_y;
                mario->tps_up = 0;
            }
        }
        else{
            
            if((mario->y - moveDistance_jump) > mario->target_y){
                    mario->y -= moveDistance_jump;
                }
            else{
                    mario->y = mario->target_y;
            }
            
        }
    }

    /* Fall */
    else if(mario->down && !mario->up){
        
        if((((mario->x - mario->rect.w/2) >= (bloc->x - bloc->rect.w/2)) && ((mario->x + mario->rect.w/2) <= (bloc->x + bloc->rect.w/2))) || (((mario->x - mario->rect.w/2) < (bloc->x + bloc->rect.w/2)) && ((mario->x + mario->rect.w/2) >= (bloc->x + bloc->rect.w/2))) || (((mario->x - mario->rect.w/2) <= (bloc->x - bloc->rect.w/2)) && ((mario->x + mario->rect.w/2) > (bloc->x - bloc->rect.w/2)))){
            
            /* To avoid the character jumping through an obstacle */
            if(((mario->y + mario->rect.h/2) <= (bloc->y - bloc->rect.h/2)) && ((mario->y + mario->rect.h/2 + moveDistance_jump) > (bloc->y - bloc->rect.h/2))){
                mario->y = bloc->y - bloc->rect.h/2 - mario->rect.h/2;
                mario->target_y = bloc->y - bloc->rect.h/2 - mario->rect.h/2;
                if(touche_up_relache) mario->obs = TRUE;
            }
            else{
                mario->y += moveDistance_jump;
            }
        }
        else{ 
            mario->y += moveDistance_jump;
            mario->obs = FALSE;
        }

        if(mario->y >= mario->previous_y){
            mario->target_y = mario->previous_y;
            mario->y = mario->previous_y;
            mario->obs = FALSE;
            if(touche_up_relache) mario->down = FALSE;
        }

    }
    
    while( SDL_PollEvent( &e ) != 0 ){
        
        /* to quit the game (by closing the window) */
        if( e.type == SDL_QUIT) *quit = TRUE;

        /* If a key if selected */
        if( e.type == SDL_KEYDOWN){

            switch( e.key.keysym.sym )
            {   
                /* to quit the game (ESC key) */
                case SDLK_ESCAPE:
                    *quit=TRUE;
                break;
            }
        }
    }

    mario->previous_x = mario->x;
    mario->dir = ' ';

    /* Movement of the character (only if the game isn't over) */
    if(!fin_jeu->lose && !fin_jeu->win){

        /* Jump (SPACE key) */
        if (touches[SDL_SCANCODE_SPACE]){

            if(mario->obs && mario->down) mario->down = !mario->down;

            if(!mario->up) Mix_PlayChannel(1,musique.saut,0);
            
            /* Memorisation of the initial y position */
            if(!mario->up && !mario->down && !mario->obs){
                mario->previous_y = mario->y;
            } 
            
                
            /* Jump (y axis)*/
            if(!mario->down || mario->obs){
                if((((mario->x - mario->rect.w/2) >= (bloc->x - bloc->rect.w/2)) && ((mario->x + mario->rect.w/2) <= (bloc->x + bloc->rect.w/2))) || (((mario->x - mario->rect.w/2) < (bloc->x + bloc->rect.w/2)) && ((mario->x + mario->rect.w/2) >= (bloc->x + bloc->rect.w/2))) || (((mario->x - mario->rect.w/2) <= (bloc->x - bloc->rect.w/2)) && ((mario->x + mario->rect.w/2) > (bloc->x - bloc->rect.w/2)))){
                    
                    /* If the character doesn't go through the obstacle by jumping */
                    if(((mario->target_y - mario->rect.h/2 - moveDistance_jump) >= (bloc->y + bloc->rect.h/2)) || ((mario->target_y + mario->rect.h/2) <= (bloc->y - bloc->rect.h/2))){
                        if((mario->target_y - moveDistance_jump - mario->rect.h/2) >= 0) 
                            mario->target_y -= moveDistance_jump;
                        else{
                            mario->target_y =  mario->rect.h/2;
                        }

                        if(mario->obs) mario->obs = FALSE;
                    }
                    else{
                        mario->target_y = bloc->y + bloc->rect.h/2 + mario->rect.h/2;
                    }
                }
                
                else if((mario->target_y - moveDistance_jump - mario->rect.h/2) >= 0) 
                    mario->target_y -= moveDistance_jump;
                else{
                    mario->target_y =  mario->rect.h/2;
                }
                
                mario->up = TRUE;
            }
        }

        /* Left (left arrow ONLY)*/
        if (touches[SDL_SCANCODE_LEFT] && !touches[SDL_SCANCODE_RIGHT]){
            mario->previous_dir = 'g';

            if((mario->x - moveDistance_walk) >= mario->rect.w/2){
                mario->dir = 'g';
                mario->x -= moveDistance_walk;
                
                if((((mario->y + mario->rect.h/2) > (bloc->y + bloc->rect.h/2)) && ((mario->y - mario->rect.h/2) < (bloc->y - bloc->rect.h/2))) || (((mario->y - mario->rect.h/2) < (bloc->y + bloc->rect.h/2)) && ((mario->y - mario->rect.h/2) > (bloc->y - bloc->rect.h/2))) || (((mario->y + mario->rect.h/2) < (bloc->y + bloc->rect.h/2)) && ((mario->y + mario->rect.h/2) > (bloc->y - bloc->rect.h/2)))){
                    if(((mario->x - mario->rect.w/2) < (bloc->x + bloc->rect.w/2)) && ((mario->x + mario->rect.w/2) > (bloc->x - bloc->rect.w/2))){
                        mario->x = mario->previous_x;
                        mario->dir = ' ';  /* arret */
                    }
                }
            }
            else{
                mario->x = mario->rect.w/2;
            }
        }

        /* Right (right arrow ONLY) */
        else if (touches[SDL_SCANCODE_RIGHT] && !touches[SDL_SCANCODE_LEFT]){
            mario->previous_dir = 'd';

            /* To avoid exiting the screen */
            if((mario->x + moveDistance_walk) <= (SCREEN_WIDTH - mario->rect.w/2)){
                mario->dir = 'd';
                
                /* Movement of the background (the character stays at the center of the screen) */
                if((mario->x >= SCREEN_WIDTH/2) && !cave->on_screen){
                     mario->x = SCREEN_WIDTH/2;
                    
                    if((((mario->y + mario->rect.h/2) > (bloc->y + bloc->rect.h/2)) && ((mario->y - mario->rect.h/2) < (bloc->y - bloc->rect.h/2))) || (((mario->y - mario->rect.h/2) < (bloc->y + bloc->rect.h/2)) && ((mario->y - mario->rect.h/2) > (bloc->y - bloc->rect.h/2))) || (((mario->y + mario->rect.h/2) < (bloc->y + bloc->rect.h/2)) && ((mario->y + mario->rect.h/2) > (bloc->y - bloc->rect.h/2)))){
                        
                        /* The character is blocked by the obstacle */
                        if(((mario->x + mario->rect.w/2) > (bloc->x - moveDistance_walk - bloc->rect.w/2)) && ((mario->x - mario->rect.w/2) < (bloc->x + bloc->rect.w/2 - moveDistance_walk))){
                            mario->dir = ' ';  /* arret */
                        }
                        else{
                            /* To avoid useless movement (when the obstacle is out of screen) */
                            if((bloc->x + bloc->rect.w/2) >= 0)
                                bloc->x -= moveDistance_walk;

                            /* Scrolling of the background */
                            Deplacement_fond(decor,moveDistance_walk);
                        }
                    }
                    else{
                        /* To avoid useless movement (when the obstacle is out of the screen) */
                        if((bloc->x + bloc->rect.w/2) >= 0)
                            bloc->x -= moveDistance_walk;
                        
                        /* The obstacle is out of the screen */
                        else{
                            bloc->onScreen = FALSE;
                        }
                        
                        /* Scrolling of the background */
                        Deplacement_fond(decor,moveDistance_walk);
                    }

                    cave->cpt_pixel += moveDistance_walk;   // Update of the pixel counter (movement)

                    /* The cave appears at the end of the trail (cpt->pixel_target) */
                    if(cave->cpt_pixel >= cave->pixel_target){  
                        cave->move = TRUE;

                        /* To insure that there are no obstacles on screen */
                        if((bloc->x + bloc->rect.w/2) <= 0){ 

                            /* Movement of the cave */
                            if((cave->rect_d.x + cave->rect_d.w - 2) > SCREEN_WIDTH){
                                cave->rect_d.x -= moveDistance_walk;
                                cave->rect_g.x -= moveDistance_walk;
                            }
                            /* The cave is FULLY displayed on screen */
                            else{
                                cave->on_screen = TRUE;
                            }
                        }
                    }

                    for(int n = 0; n < MAX_ENNEMIS; n++){
                        if(ennemis[n]->isAlive) ennemis[n]->x -= moveDistance_walk;
                    }
                }

                else{

                    if((((mario->y + mario->rect.h/2) > (bloc->y + bloc->rect.h/2)) && ((mario->y - mario->rect.h/2) < (bloc->y - bloc->rect.h/2))) || (((mario->y - mario->rect.h/2) < (bloc->y + bloc->rect.h/2)) && ((mario->y - mario->rect.h/2) > (bloc->y - bloc->rect.h/2))) || (((mario->y + mario->rect.h/2) < (bloc->y + bloc->rect.h/2)) && ((mario->y + mario->rect.h/2) > (bloc->y - bloc->rect.h/2)))){
                        if(((mario->x + mario->rect.w/2 + moveDistance_walk) > (bloc->x - bloc->rect.w/2)) && ((mario->x - mario->rect.w/2 + moveDistance_walk) < (bloc->x + bloc->rect.w/2))){
                            mario->dir = ' ';  /* arret */
                        }
                        else{
                            mario->x += moveDistance_walk;
                        }
                    }
                    else{ mario->x += moveDistance_walk;}

                    /* If the character enters the cave (game won) */
                    if(!mario->exit_game && ((mario->x - mario->rect.w/2) >= cave->rect_g.x)){ 
                        mario->exit_game = TRUE;
                        Mix_PlayChannel(0, musique.WIN_SOUND_EFFECT, 0);
                    }
                }

                /* If the obstacle exits the screen and that BOTH the cave and the bird aren't on screen */
                if(!cave->move && !bloc->onScreen){
                    int pos = rand();
                    bloc->x = SCREEN_WIDTH + bloc->rect.w/2;
                    pos %= 100;
                    if(pos < 50) bloc->y = SCREEN_HEIGHT/2;    // Position 1
                    else{ bloc->y = 3*SCREEN_HEIGHT/4; }       // Position 2
                    bloc->onScreen = TRUE;  
                    
                }
            }
            else{
                mario->x = SCREEN_WIDTH - mario->rect.w/2;
            }
        }

        if(mario->dir == ' ') Mix_HaltChannel(1);
    }
}

/* Initialize the main character */
void Init_personnage(SDL_Renderer* ecran, Personnage* mario){

    if(mario){ 
        mario->texture_d[0] = creerTextureImg(ecran,"Images/mario_d_1.bmp",&mario->rect.w,&mario->rect.h);  
        mario->texture_d[1] = creerTextureImg(ecran,"Images/mario_d_2.bmp",NULL,NULL);
        mario->texture_g[0] = creerTextureImg(ecran,"Images/mario_g_1.bmp",NULL,NULL);
        mario->texture_g[1] = creerTextureImg(ecran,"Images/mario_g_2.bmp",NULL,NULL);
        mario->texture_saut[0] = creerTextureImg(ecran,"Images/PERSO_DROITE_SAUT.bmp",NULL,NULL);
        mario->texture_saut[1] = creerTextureImg(ecran,"Images/PERSO_GAUCHE_SAUT.bmp",NULL,NULL);
        mario->texture_arret[0] = creerTextureImg(ecran,"Images/PERSO_DROITE_ARRET.bmp",NULL,NULL);
        mario->texture_arret[1] = creerTextureImg(ecran,"Images/PERSO_GAUCHE_ARRET.bmp",NULL,NULL);
        mario->texture_mort = creerTextureImg(ecran, "Images/mario_mort.bmp",NULL,NULL);
        mario->rect.w /= 3;
        mario->rect.h /= 3;
        mario->y = SCREEN_HEIGHT - (mario->rect.h/2 + 15);
        mario->x = 3*SCREEN_WIDTH/4 + mario->rect.w/2;
        mario->up = FALSE;
        mario->down = FALSE;
        mario->exit_menu = FALSE;
        mario->target_y = mario->y;
        mario->dir = ' ';
        mario->previous_dir = 'd';
        mario->previous_y = mario->y;
        mario->previous_x = mario->x;
        mario->alternance = FALSE;
        mario->tps_up = 0;
        mario->obs = FALSE;
        mario->exit_game = FALSE;
        mario->isAlive = TRUE;
    }
}

/* Display the character on screen */
void dessine_personnage(SDL_Renderer* ecran, Personnage* mario){
    mario->rect.x = mario->x - mario->rect.w/2;
    mario->rect.y = mario->y - mario->rect.h/2;

    /* If the character is alive */
    if(mario->isAlive){
    
        /* Display the character in different positions depending of the situation */
        /* Walking to the right */
        if(mario->dir == 'd'){
            if(!mario->obs && (mario->previous_y != mario->y)){
                dessine_Img_redim(ecran,mario->texture_saut[0], mario->rect);
            }
            else{
                if(mario->alternance) dessine_Img_redim(ecran,mario->texture_d[1],mario->rect);
                else { dessine_Img_redim(ecran,mario->texture_d[0],mario->rect); }
            }
        }

        /* Walking to the left */
        else if(mario->dir == 'g'){
            if(!mario->obs && (mario->previous_y != mario->y)){
                dessine_Img_redim(ecran,mario->texture_saut[1], mario->rect);
            }
            else{
                if(mario->alternance) dessine_Img_redim(ecran,mario->texture_g[1],mario->rect);
                else { dessine_Img_redim(ecran,mario->texture_g[0],mario->rect); }
            }
        }

        /* Standing still */
        else{
        
            if(mario->previous_dir == 'd'){
                if(!mario->obs && (mario->previous_y != mario->y)){
                    dessine_Img_redim(ecran,mario->texture_saut[0], mario->rect);
                }
                else{
                    dessine_Img_redim(ecran,mario->texture_arret[0],mario->rect);
                }
            }
            else if(mario->previous_dir =='g'){
                if(!mario->obs && (mario->previous_y != mario->y)){
                    dessine_Img_redim(ecran,mario->texture_saut[1], mario->rect);
                }
                else{
                    dessine_Img_redim(ecran,mario->texture_arret[1],mario->rect);
                }
            }
        }
    }

    /* If the character is dead */
    else{
        
        /* Modifications for the "dead" image (to do only once) */
        if(mario->rect.w < mario->rect.h){
            int rect_tmp = mario->rect.w;
            mario->rect.w = mario->rect.h;
            mario->rect.h = rect_tmp;
            if(!mario->obs) mario->y = SCREEN_HEIGHT - (mario->rect.h/2 + 5);
            mario->rect.y = mario->y - mario->rect.h/2;
        }

        dessine_Img_redim(ecran,mario->texture_mort, mario->rect);
    }
}

/* Check if the character bumps into an enemy  */
int checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < (b.x + b.w)) &&
            ((a.x + a.w) > b.x) &&
            (a.y < (b.y + b.h)) &&
            ((a.y + a.h) > b.y);
}

/* Free the textures */
void free_mario(Personnage* mario){
    if(mario){
        if(mario->texture_arret[0]){SDL_DestroyTexture(mario->texture_arret[0]); mario->texture_arret[0] = NULL;}
        if(mario->texture_arret[1]){SDL_DestroyTexture(mario->texture_arret[1]); mario->texture_arret[1] = NULL;}
        if(mario->texture_d[0]){SDL_DestroyTexture(mario->texture_d[0]); mario->texture_d[0] = NULL;}
        if(mario->texture_d[1]){SDL_DestroyTexture(mario->texture_d[1]); mario->texture_d[1] = NULL;}
        if(mario->texture_g[0]){SDL_DestroyTexture(mario->texture_g[0]); mario->texture_g[0] = NULL;}
        if(mario->texture_g[1]){SDL_DestroyTexture(mario->texture_g[1]); mario->texture_g[1] = NULL;}
        if(mario->texture_saut[0]){SDL_DestroyTexture(mario->texture_saut[0]); mario->texture_saut[0] = NULL;}
        if(mario->texture_saut[1]){SDL_DestroyTexture(mario->texture_saut[1]); mario->texture_saut[1] = NULL;}
        if(mario->texture_mort){SDL_DestroyTexture(mario->texture_mort); mario->texture_mort = NULL;}
    }
}
