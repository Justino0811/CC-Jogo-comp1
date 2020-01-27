#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int running = 1;

int main(int argc, char* args[]){

	SDL_Window* window = NULL;
	SDL_Surface* tela = NULL;
	SDL_Surface* fundo = NULL;

	window = SDL_CreateWindow( "Jogo", SDL_WINDOWPOS_UNDEFINED, 0, 700, 700, SDL_WINDOW_SHOWN);
	
	tela = SDL_GetWindowSurface( window );
	fundo = IMG_Load("menu.png");
	
	while(running){
		SDL_BlitSurface(fundo, NULL, tela, NULL);
		SDL_UpdateWindowSurface( window );
		SDL_Delay(10000);
		break;
	}

	SDL_FreeSurface(fundo);
	SDL_DestroyWindow( window );
	SDL_Quit();
return 0;
}