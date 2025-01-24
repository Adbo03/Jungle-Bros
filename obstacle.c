#include "obstacle.h"

void Init_obstacle(SDL_Renderer* ecran, Obstacle* obs){ 

    if(obs){    
        obs->texture = creerTextureImg(ecran,"Images/buche.bmp",&obs->rect.w,&obs->rect.h);
        obs->rect.w /= 1.5 ;
        obs->rect.h /= 2;
        obs->rect.x = SCREEN_WIDTH + obs->rect.w/2;
        obs->rect.y = SCREEN_HEIGHT/2 - obs->rect.h/2;
        obs->y = obs->rect.y + obs->rect.h/2;
        obs->x = obs->rect.x + obs->rect.w/2;
        obs->onScreen = TRUE;

    }
}

/* Dessine l'obstacle sur l'ecran */
void dessine_obstacle(SDL_Renderer* ecran, Obstacle obs){

    obs.rect.x = obs.x - obs.rect.w/2;
    obs.rect.y = obs.y - obs.rect.h/2;
    dessine_Img_redim(ecran,obs.texture,obs.rect);
}


