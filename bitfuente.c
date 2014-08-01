#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "savepng.h"

SDL_Surface *screen;
SDL_Surface *canvas;
SDL_Surface *letra;


SDL_Event event;

SDL_Color colorletra;
SDL_Color colorborde;

TTF_Font *font = NULL;

SDL_Rect rect;

int font_size = 1;

int ancho_imagen = 0;

int ancho_tile;

int outline_size;

int posx,posy;

int init()
{

	if(SDL_Init(SDL_INIT_VIDEO)==-1)return 1;

    screen = SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);

	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,255,0,255));

    if(screen==NULL)return 1;

	if(TTF_Init()==-1)return 1;

	Uint32 rmask, gmask, bmask, amask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    	rmask = 0xff000000;
    	gmask = 0x00ff0000;
    	bmask = 0x0000ff00;
    	amask = 0x000000ff;
	#else
    	rmask = 0x000000ff;
    	gmask = 0x0000ff00;
    	bmask = 0x00ff0000;
    	amask = 0xff000000;
	#endif

	canvas = SDL_CreateRGBSurface(0,ancho_imagen,ancho_imagen,32,rmask,gmask,bmask,amask);


	
}


void apply_surface(SDL_Surface *origen,SDL_Surface *destino,int x,int y)
{

	SDL_Rect temp;
	temp.x = x;
	temp.y = y;
		

    SDL_BlitSurface(origen,NULL,destino,&temp);

}



SDL_Surface* dibujarletra(char* t)
{

	SDL_Surface* out = NULL;
	SDL_Surface* black_letters = NULL;
	SDL_Surface* white_letters = NULL;
	SDL_Surface* bg = NULL;
	SDL_Rect dstrect;

	Uint32 rmask, gmask, bmask, amask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    	rmask = 0xff000000;
    	gmask = 0x00ff0000;
    	bmask = 0x0000ff00;
    	amask = 0x000000ff;
	#else
    	rmask = 0x000000ff;
    	gmask = 0x0000ff00;
    	bmask = 0x00ff0000;
    	amask = 0xff000000;
	#endif

	Uint32 color_key;

	TTF_SetFontOutline(font, outline_size);

	black_letters = TTF_RenderText_Solid(font, t, colorborde);

	bg = SDL_CreateRGBSurface(0,ancho_tile,ancho_tile,32,rmask,gmask,bmask,amask);

	/* Use color key for eventual transparency: */
	color_key = SDL_MapRGB(bg->format, 01, 01, 01);
	SDL_FillRect(bg, NULL, color_key);

	dstrect.x = posx;
	dstrect.y = posy;
	
	SDL_BlitSurface(black_letters , NULL, bg, &dstrect );

	SDL_FreeSurface(black_letters);

	/* --- Put the color version of the text on top! --- */

	TTF_SetFontOutline(font, 0);

	white_letters = TTF_RenderText_Solid(font, t, colorletra);

	dstrect.x = posx + outline_size;
	dstrect.y = posy + outline_size;
	SDL_BlitSurface(white_letters, NULL, bg, &dstrect);
	SDL_FreeSurface(white_letters);

	/* --- Convert to the screen format for quicker blits --- */
	SDL_SetColorKey(bg, SDL_SRCCOLORKEY|SDL_RLEACCEL, color_key);
	out = SDL_DisplayFormatAlpha(bg);
	SDL_FreeSurface(bg);

	return out;

} 


int main(int argc, char* argv[])
{

	int quit = 0;
	int k;

	for(k=1;k < 13;k++)
	{
		if(argv[k] == NULL)
		{	
			printf("usage:bitfuente ancho-tile tamaño-fuente posx posy tamaño-outline color-letra color-borde path-fuente\n");
			return 1;
		}
	}	

	ancho_tile = atoi(argv[1]);

	ancho_imagen = ancho_tile*16;

	font_size = atoi(argv[2]);

	init();

	posx = atoi(argv[3]);
	posy = atoi(argv[4]);

	outline_size = atoi(argv[5]);

	colorletra = {atoi(argv[6]),atoi(argv[7]),atoi(argv[8])};
	colorborde = {atoi(argv[9]),atoi(argv[10]),atoi(argv[11])};

	font = TTF_OpenFont(argv[12],font_size);

	int i,j;

	int ascii = 0;
	char buffer[2];

	
	for(j=0;j<16;j++)
	{
		for(i=0;i<16;i++)
		{
				if(((ascii>=33)&&(ascii<127))||(ascii>=160))
				{

					sprintf(buffer,"%c",ascii);
					//letra = TTF_RenderText_Solid(font,buffer,textColor);
					letra = dibujarletra(buffer);
					apply_surface(letra,canvas,ancho_tile*i,ancho_tile*j);

				}

				ascii++;
					
		}	
	}

	apply_surface(canvas,screen,0,0);

	SDL_Flip(screen);

	SDL_SavePNG(canvas,"fuente.png");

	while( quit == 0 )
    {
        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = 1;
            }
        }
    }

    SDL_FreeSurface(letra);
    SDL_FreeSurface(canvas);
    SDL_FreeSurface(screen);

    TTF_CloseFont(font);

    TTF_Quit();



	return 0;

}

