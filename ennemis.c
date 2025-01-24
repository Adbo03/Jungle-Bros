#include "ennemis.h"   // Inclut les déclarations et la structure Ennemi
#include "stdio.h"    

// Initialise un ennemi avec ses propriétés de base
Ennemi* initEnnemi(SDL_Renderer* ecran, int type) {
    Ennemi *ennemi = (Ennemi*) malloc(sizeof(Ennemi));

    /* Chargement des textures pour l'animation et initialisation des variables pour la gestion de l'ennemi */
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

    /* Mise à jour de la variable SDL_Rect pour l'affichage */
    ennemi->rect.x = ennemi->x - ennemi->rect.w / 2;
    ennemi->rect.y = ennemi->y - ennemi->rect.h / 2;

    /* Affichage de l'ennemi a l'ecran */
    SDL_RenderCopy(ecran, ennemi->textures[ennemi->currentFrame], NULL, &(ennemi->rect));
}

/* Met à jour les coordonnees et des etats des ennemis */
void updateEnnemi(Ennemi* ennemi[MAX_ENNEMIS], double elapsed, Effets_sonores* musique, Caverne cave, Obstacle obs) {

    /* Mise à jour des positions de TOUS les ennemis */
    for(int n = 0; n < MAX_ENNEMIS; n++){
        double moveDistance = ennemi[n]->speed * (elapsed / 1000.0); // Calcule la distance de déplacement
        
        if (ennemi[n]->isAlive) {
            
            /* Mise à jour de la position de l'oiseau */
            if (ennemi[n]->type == TYPE_OISEAU) {
                if (ennemi[n]->y < COORD_SOL) {
                    if(!Mix_Playing(2)) Mix_PlayChannel(2, musique->oiseau, 0);
                    ennemi[n]->y += moveDistance ;   // Déplacement vertical
                    ennemi[n]->x -= moveDistance;
                    
                    /* Si l'oiseau traverse un bloc */
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
                    ennemi[n]->speed += 3.0;         // L'oiseau accélère
                }
            }

            /* Mise à jour de la position de l'araignée */
            else if (ennemi[n]->type == TYPE_ARRAIGNE) {
                ennemi[n]->jumpTimer += elapsed / 1000.0; // Incrémente le temps écoulé
                ennemi[n]->x -= moveDistance;

                if (ennemi[n]->isJumping) {
                    /* Calcul du mouvement en cloche (utilise un sinus pour le saut) */
                    float progress = ennemi[n]->jumpTimer / 1.5; // Durée totale du saut
                    if (progress > 1.0) {
                        // Fin du saut
                        ennemi[n]->isJumping = 0;
                        ennemi[n]->jumpTimer = 0;
                    } else {
                        // Position pendant le saut
                        ennemi[n]->y = SCREEN_HEIGHT - (ennemi[n]->rect.h/2 + 15) - ennemi[n]->jumpHeight * sin(progress * PI); // Mouvement vertical
                    }
                    
                    /* Si l'arraignee traverse un bloc */
                    ennemi[n]->rect.x = ennemi[n]->x - ennemi[n]->rect.w/2;
                    ennemi[n]->rect.y = ennemi[n]->y - ennemi[n]->rect.h/2;
                    obs.rect.x = obs.x - obs.rect.w/2;
                    obs.rect.y = obs.y - obs.rect.h/2;
                    if(checkCollision(ennemi[n]->rect,obs.rect)){
                        ennemi[n]->y = obs.rect.y + obs.rect.h + ennemi[n]->rect.h/2;
                    }
                } 
                else if (ennemi[n]->jumpTimer >= 3.0) {

                    /* Déclenchement d'un nouveau saut */
                    ennemi[n]->isJumping = 1;
                    ennemi[n]->jumpTimer = 0;
                    ennemi[n]->jumpStartX = ennemi[n]->x;
                    if(!Mix_Playing(2)) Mix_PlayChannel(2, musique->arraigne, 0);
                }
            }
        } 
        
        /* Vérifie si l'ennemi sort de l'écran */
        if ((ennemi[n]->x + ennemi[n]->rect.w / 2) < 0) {
            ennemi[n]->x = (SCREEN_WIDTH) + (ennemi[n]->rect.w/2);
            if(ennemi[n]->type == TYPE_OISEAU) ennemi[n]->y = SCREEN_HEIGHT/2 - ennemi[n]->rect.h/2; 
            ennemi[n]->isAlive = FALSE;
            Mix_HaltChannel(2);
        }

        if(!check_Alive(ennemi) && !cave.move && !cave.on_screen && (!ennemi[n]->isAlive)){
            int luck = rand()%100;

            /* Apparition aleatoire des ennemis */
            if(((ennemi[n]->type == TYPE_OISEAU) && ( luck <= 1)) || ((ennemi[n]->type == TYPE_ARRAIGNE) && ( luck <= 3))){
                ennemi[n]->isAlive=TRUE;
                ennemi[n]->speed_start += 25;
                ennemi[n]->speed = ennemi[n]->speed_start;
            }
        }
    }
}

/* Verifie si un ennemi est affiché à l'ecran */
int check_Alive(Ennemi* ennemis[MAX_ENNEMIS]){
    for(int n = 0; n < MAX_ENNEMIS; n++){
        if(ennemis[n]->isAlive)  return TRUE;
    }

    return FALSE;
}

void updateAnimation(Ennemi* ennemi) {
    if (ennemi->textureCount > 0) {
        // Incrémente le compteur
        ennemi->animationCounter++;

        // Si le délai est atteint, met à jour le cadre
        if (ennemi->animationCounter >= ennemi->animationDelay) {
            ennemi->currentFrame = (ennemi->currentFrame + 1) % ennemi->textureCount;
            ennemi->animationCounter = 0;  // Réinitialise le compteur
        }
    }
}

/* Destruction des textures et desallocation de la mémoire allouée */
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