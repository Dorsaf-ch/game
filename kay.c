#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL_image.h" 
#include "SDL/SDL_ttf.h" 
#include <SDL/SDL_mixer.h> 
#include "kay.h"
#include <math.h>

void init_kay(Kay *kay)
{
	kay->posinit=160;
	kay->poskay.x=150;
	kay->poskay.y=kay->posinit;
	kay->acc=0;
	kay->v=3;

	kay->moving=0;
	kay->hauteur=50;
	kay->saut=0;
	kay->hauteurAtteint=0;
}

void deplacerKay(Kay *kay, int keysHeld[], int *prev)
{
	kay->moving=0;
      
               if (keysHeld[SDLK_UP]) 
		{
               		kay->poskay.y-=2;
		}
		

               if (keysHeld[SDLK_DOWN])
		{
               		kay->poskay.y+=2;
		}

               if (keysHeld[SDLK_RIGHT])
		{
			calculdx(kay);
              		kay->poskay.x+=kay->dx;
			
			*prev = SDLK_RIGHT;
			kay->moving=1;
		}


               if (keysHeld[SDLK_LEFT])
		{
			calculdx(kay);
             		kay->poskay.x-=kay->dx;
			
			*prev = SDLK_LEFT;
			kay->moving=1;
		}

		if(kay->moving == 0)
		{
			if(*prev==SDLK_RIGHT)
			{
				calculdx(kay);
              			kay->poskay.x+=kay->dx;
			}
			if(*prev==SDLK_LEFT)
			{
				calculdx(kay);
              			kay->poskay.x-=kay->dx;
			}
		}          
}


/*void deplacerSouris(Kay *kay, SDL_Event event)
{
	kay->poskay.x = event.button.x - kay->imgkay[0]->w / 2;
        kay->poskay.y = event.button.y - kay->imgkay[0]->h / 2;
}*/

void deplacerSouris(Kay *kay, int pos_souris)
{
	kay->moving =1;
	calculdx(kay);
	if(kay->poskay.x < pos_souris)
	{	
		if(kay->poskay.x + kay->dx <= pos_souris)
		{
			kay->poskay.x += kay->dx;
		}
		else
		{
			kay->poskay.x = pos_souris;
		}
	}
	
	else if(kay->poskay.x > pos_souris)
	{
		if(kay->poskay.x - kay->dx >= pos_souris)
		{
			kay->poskay.x -= kay->dx;
		}
		else
		{
			kay->poskay.x = pos_souris;
		}
	}
}

void calculdx(Kay *kay)
{
	//int max=100, min=10, x=kay->acc+kay->v;
	//kay->dx=(1/2) * kay->acc * dt * dt + kay->v * dt;
	kay->dx= kay->acc + kay->v;
	/*if x>max 
	{	
		kay->dx=max;
	}
	else if x<min
	kay->dx=min;
	else
	kay->dx=x;*/
}

void saut(Kay *kay, int keysHeld[])
{
	if(keysHeld[SDLK_r] && kay->saut == 0)
	{
		kay->saut = 1;
	}
	if(kay->saut == 1)
	{
		//kay->poskay.y += 1;
		if(kay->hauteurAtteint == 0)
		{
			kay->poskay.y -= 4;
		}
		else
		{
			kay->poskay.y += 4;
		}
		if(kay->posinit - kay->poskay.y >= kay->hauteur)
		{
			kay->hauteurAtteint = 1;
		}
		if(kay->hauteurAtteint == 1 && kay->posinit - kay->poskay.y <= 0)
		{
			kay->hauteurAtteint = 0;
			kay->saut = 0;
		}
	}
		
}

int displaytext(SDL_Surface* screen, TTF_Font* font, Kay kay)
{ 
    char text[100];
    sprintf(text, "hauteurAtteint = %d saut = %d, y = %d", kay.hauteurAtteint, kay.saut, kay.poskay.y);
    SDL_Color black = {255,0,0};
    SDL_Surface *text_surface;

    SDL_Rect position;
    position.x=0;
    position.y=30;
    
    text_surface = TTF_RenderText_Blended(font, text, black); 
    if (text_surface != NULL)
    {
        SDL_BlitSurface(text_surface, NULL, screen, &position);

        return 1;
    }
    else
    {
        // report error
        return 0;
    }
}

void animate(Kay kay, int *x, int *n)
{
	if(fabs(kay.poskay.x - (*x)) > 10)
	{
		(*x) = kay.poskay.x;
		(*n)++;
		if(*n > 2)
		{
			*n=0;
		}
	}
}
