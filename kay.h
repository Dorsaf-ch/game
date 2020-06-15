#ifndef ENNEMI_H_INCLUDED
#define ENNEMI_H_INCLUDED

#include<SDL/SDL.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

typedef struct 
{
	SDL_Surface *imgkay;
	SDL_Surface *imghp;
	SDL_Rect poskay;
	double v;
	double acc;
	int hp;
	double dx;
	int hauteur, saut, hauteurAtteint, posinit;
	int moving;
    	SDL_Rect poshp;
	//vie kayvie;
} Kay;


void init_kay(Kay *kay);
void deplacerKay(Kay *kay, int keysHeld[], int *prev);
void deplacerSouris(Kay *kay, int pos_souris);
int displaytext(SDL_Surface* screen, TTF_Font* font, Kay kay);
void calculdx(Kay *kay);
void saut(Kay *kay, int keysHeld[]);
//void scroll(Kay *kay, Camera *cam);

#endif

