#include "ennemis.h"   
#include "stdio.h"    

/* Initialization of an ennemy */
Ennemi* initEnnemi(SDL_Renderer* ecran, int type) {
    Ennemi *ennemi = (Ennemi*) malloc(sizeof(Ennemi));

    /* Loading of the textures and initialization of the variables to manage the ennemy */
    if (type == TYPE_OISEAU){ 
        ennemi->textureCount = 4; 
        ennemi->textures = malloc(ennemi->textureCount * sizeof(SDL_Texture*));
        ennemi->textures[0] = creerTextureImg(ecran, "Images/ennemi-oiseau1.bmp", &(ennemi->rect.w), &(ennemi->rect.h));
        ennemi->textures[1] = creerTextureImg(ecran, "Images/ennemi-oiseau2.bmp", &(ennemi->rect.w), &(ennemi->rect.h));
        ennemi->textures[2] = creerTextureImg(ecran, "Images/ennemi-oiseau3.bmp", &(ennemi->rect.w), &(ennemi->rect.h));
        ennemi->textures[3] = creerTextureImg(ecran, "Images/ennemi-oiseau4.bmp", &(ennemi->rect.w), &(ennemi->rect.h));
        ennemi->rect.w = ennemi->rect.w/4;
        ennemi->rect.h = ennemi->rect.h/4;
        ennemi->x = (SCREEN_WIDTH + 200) - (ennemi->rect.w/2);
        ennemi->y = SCREEN_HEIGHT/2 - ennemi->rect.h/2;
        ennemi->speed_start = 120;
        ennemi->speed = ennemi->speed_start;
    }
    if (type == TYPE_ARRAIGNE){
        ennemi->textureCount = 5; 
        ennemi->textures = malloc(ennemi->textureCount * sizeof(SDL_Texture*));
        ennemi->textures[0] = creerTextureImg(ecran, "Images/ennemi-arraigne1.bmp", &(ennemi->rect.w), &(ennemi->rect.h));
        ennemi->textures[1] = creerTextureImg(ecran, "Images/ennemi-arraigne2.bmp", &(ennemi->rect.w), &(ennemi->rect.h));
        ennemi->textures[2] = creerTextureImg(ecran, "Images/ennemi-arraigne3.bmp", &(ennemi->rect.w), &(ennemi->rect.h));
        ennemi->textures[3] = creerTextureImg(ecran, "Images/ennemi-arraigne4.bmp", &(ennemi->rect.w), &(ennemi->rect.h));
        ennemi->textures[4] = creerTextureImg(ecran, "Images/ennemi-arraigne5.bmp", &(ennemi->rect.w), &(ennemi->rect.h));
        ennemi->rect.w = (ennemi->rect.w)*0.5;
        ennemi->rect.h = (ennemi->rect.h)*0.5;
        ennemi->x = SCREEN_WIDTH + 200;
        ennemi->y = SCREEN_HEIGHT - (ennemi->rect.h/2 + 15);
        ennemi->speed_start = 100;
        ennemi->speed = ennemi->speed_start;
        ennemi->jumpTimer = 0;    
        ennemi->isJumping = 0;    
        ennemi->jumpStartX = ennemi->x;
        ennemi->jumpHeight = 100; 
    }
    ennemi->type = type;
    ennemi->isAlive = FALSE;        
    ennemi->currentFrame = 0;
    ennemi->animationDelay = 20; 
    ennemi->animationCounter = 0;

    return ennemi;
}

void dessine_Ennemis(Ennemi* ennemi, SDL_Renderer* ecran) {

    /* Update of the SDL_Rect variable for the display */
    ennemi->rect.x = ennemi->x - ennemi->rect.w / 2;
    ennemi->rect.y = ennemi->y - ennemi->rect.h / 2;

    /* Display of the ennemy on screen */
    SDL_RenderCopy(ecran, ennemi->textures[ennemi->currentFrame], NULL, &(ennemi->rect));
}

/* Update the position and the state (alive/dead) of the ennemies */
void updateEnnemi(Ennemi* ennemi[MAX_ENNEMIS], double elapsed, Effets_sonores* musique, Caverne cave, Obstacle obs) {

    /* Update the positions of ALL ennemies */
    for(int n = 0; n < MAX_ENNEMIS; n++){
        double moveDistance = ennemi[n]->speed * (elapsed / 1000.0); // Calculation of the speed of movement
        
        if (ennemi[n]->isAlive) {
            
            /* Update the position of the eagle */
            if (ennemi[n]->type == TYPE_OISEAU) {
                if (ennemi[n]->y < COORD_SOL) {
                    if(!Mix_Playing(2)) Mix_PlayChannel(2, musique->oiseau, 0);
                    ennemi[n]->y += moveDistance ;   
                    ennemi[n]->x -= moveDistance;
                    
                    /* If the eagle goes through an obstacle */
                    ennemi[n]->rect.x = ennemi[n]->x - ennemi[n]->rect.w/2;
                    ennemi[n]->rect.y = ennemi[n]->y - ennemi[n]->rect.h/2;
                    obs.rect.x = obs.x - obs.rect.w/2;
                    obs.rect.y = obs.y - obs.rect.h/2;
                    if(checkCollision(ennemi[n]->rect,obs.rect)){
                        ennemi[n]->x -= moveDistance;   
                        ennemi[n]->y = obs.rect.y - ennemi[n]->rect.h/2;
                    }
                } 
                else {
                    ennemi[n]->x -= moveDistance;
                    ennemi[n]->speed += 3.0;         // The eagle speeds up
                }
            }

            /* Update the position of the spider */
            else if (ennemi[n]->type == TYPE_ARRAIGNE) {
                ennemi[n]->jumpTimer += elapsed / 1000.0; // Incrementation by the elapsed time
                ennemi[n]->x -= moveDistance;

                if (ennemi[n]->isJumping) {
                    /* Calculation for the jump (using a sinus) */
                    float progress = ennemi[n]->jumpTimer / 1.5; // Total duration of a jump
                    if (progress > 1.0) {
                        // End of the jump
                        ennemi[n]->isJumping = 0;
                        ennemi[n]->jumpTimer = 0;
                    } else {
                        // Position pendant le saut
                        ennemi[n]->y = SCREEN_HEIGHT - (ennemi[n]->rect.h/2 + 15) - ennemi[n]->jumpHeight * sin(progress * PI); // Vertical movement
                    }
                    
                    /* If the spider goes through an obstacle */
                    ennemi[n]->rect.x = ennemi[n]->x - ennemi[n]->rect.w/2;
                    ennemi[n]->rect.y = ennemi[n]->y - ennemi[n]->rect.h/2;
                    obs.rect.x = obs.x - obs.rect.w/2;
                    obs.rect.y = obs.y - obs.rect.h/2;
                    if(checkCollision(ennemi[n]->rect,obs.rect)){
                        ennemi[n]->y = obs.rect.y + obs.rect.h + ennemi[n]->rect.h/2;
                    }
                } 
                else if (ennemi[n]->jumpTimer >= 3.0) {

                    /* Unleashing of a new jump */
                    ennemi[n]->isJumping = 1;
                    ennemi[n]->jumpTimer = 0;
                    ennemi[n]->jumpStartX = ennemi[n]->x;
                    if(!Mix_Playing(2)) Mix_PlayChannel(2, musique->arraigne, 0);
                }
            }
        } 
        
        /* Check if the ennemy is still on screen */
        if ((ennemi[n]->x + ennemi[n]->rect.w / 2) < 0) {
            ennemi[n]->x = (SCREEN_WIDTH) + (ennemi[n]->rect.w/2);
            if(ennemi[n]->type == TYPE_OISEAU) ennemi[n]->y = SCREEN_HEIGHT/2 - ennemi[n]->rect.h/2; 
            ennemi[n]->isAlive = FALSE;
            Mix_HaltChannel(2);
        }

        if(!check_Alive(ennemi) && !cave.move && !cave.on_screen && (!ennemi[n]->isAlive)){
            int luck = rand()%100;

            /* Random appearances of the ennemy */
            if(((ennemi[n]->type == TYPE_OISEAU) && ( luck <= 1)) || ((ennemi[n]->type == TYPE_ARRAIGNE) && ( luck <= 3))){
                ennemi[n]->isAlive=TRUE;
                ennemi[n]->speed_start += 25;
                ennemi[n]->speed = ennemi[n]->speed_start;
            }
        }
    }
}

/* Check if the ennemy is still on screen */
int check_Alive(Ennemi* ennemis[MAX_ENNEMIS]){
    for(int n = 0; n < MAX_ENNEMIS; n++){
        if(ennemis[n]->isAlive)  return TRUE;
    }

    return FALSE;
}

void updateAnimation(Ennemi* ennemi) {
    if (ennemi->textureCount > 0) {
        // Incrementation of the counter
        ennemi->animationCounter++;

        // If the time is up, update the frame
        if (ennemi->animationCounter >= ennemi->animationDelay) {
            ennemi->currentFrame = (ennemi->currentFrame + 1) % ennemi->textureCount;
            ennemi->animationCounter = 0;  // Réinitialise le compteur
        }
    }
}

/* Destruction of the textures and free of the allocated memory */
void free_ennemis(Ennemi** ennemi) {

    if ((*ennemi)->textures != NULL) { 
        for (int i = 0; i < (*ennemi)->textureCount; i++) {
            if ((*ennemi)->textures[i] != NULL) { 
                SDL_DestroyTexture((*ennemi)->textures[i]);
            }
        }
        free((*ennemi)->textures); 
        (*ennemi)->textures = NULL; 
    }
    (*ennemi)->textureCount = 0;
    free(*ennemi);
    *ennemi = NULL;
    
}
