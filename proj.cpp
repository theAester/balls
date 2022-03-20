#include "SDLlibs.h"

SDL_Texture* my_load_img_texture(SDL_Renderer* renderer, const char* filename){
	SDL_Surface* temp = IMG_Load(filename);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	return text;
}

void Load_Assets(SDL_Renderer* renderer){

}

void Unload_Assets(){

}
