#include "kay.h"
#include<SDL/SDL.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

int main(int argc, char *argv[])
{
	int FPS=1, i, pos_souris, souris=1;
	int prev = 0;
	int continuer=1;
	int keysHeld[400];
        TTF_Init();
        TTF_Font* font = TTF_OpenFont("angelina.ttf", 40);
	SDL_Event event;
	SDL_Surface *ecran=NULL, *background=NULL;
	SDL_Rect posbg;
	Kay kay;

	
	SDL_Init(SDL_INIT_VIDEO);

	ecran = SDL_SetVideoMode(2048,252, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);

	init_kay(&kay);
	pos_souris=kay.poskay.x;
	SDL_EnableKeyRepeat(10, 10);
	background= IMG_Load("b.png");
	kay.imgkay=IMG_Load("00.png");

	for(i=0; i<400; i++)
	{
		keysHeld[i]=0;
	}
	while (continuer) 
	{	
	
		//t_prev = SDL_GetTicks();

		while(SDL_PollEvent(&event))
		{
   		switch(event.type) 
   		{
    			case SDL_QUIT: 
    				continuer = 0;
    	 			break;

		    	case SDL_KEYDOWN:
				keysHeld[event.key.keysym.sym] = 1;
				break;

			case SDL_KEYUP:
				keysHeld[event.key.keysym.sym] = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				pos_souris = event.button.x - kay.imgkay->w / 2;
				break;
		}
		}
		
		// fct frottement
		if (kay.dx>0 && kay.moving == 0)
		{
			kay.acc-=0.04;
		}
		if (kay.dx<=0 && kay.moving == 0)
		{
			prev = 0;
			kay.acc=0;
			kay.dx=0;
		}
		//
		// acc 
		if (keysHeld[SDLK_a])
		{
			kay.acc += 0.04;
		}
		else if(kay.acc>0 && kay.moving == 1)
		{
			kay.acc -= 0.04;
		}//
		//deceleration
		if (keysHeld[SDLK_SPACE] && kay.dx > 0)
		{
			kay.acc -= 0.02;
		}
		else if(kay.moving == 1 && kay.acc < 0)
		{
			kay.acc += 0.02;
		}
		if(souris == 1)
		{
			deplacerSouris(&kay, pos_souris);
		}
		else
		{
			deplacerKay(&kay, keysHeld, &prev);
		}
		saut(&kay, keysHeld);
		
		SDL_FillRect(ecran,  NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
   		SDL_BlitSurface(background, NULL, ecran, NULL);
		SDL_BlitSurface(kay.imgkay, NULL, ecran, &(kay.poskay));
		
		//displaytext(ecran, font, kay);

		SDL_Flip(ecran);

		//dt=SDL_GetTicks() - t_prev;
		//if(1000/FPS > dt)
		//{
		//	SDL_Delay(1000/FPS - dt);
		//}

	}

    SDL_FreeSurface(kay.imgkay);
    SDL_FreeSurface(ecran);
    SDL_Quit();
    return EXIT_SUCCESS;
}

