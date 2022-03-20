#include <iostream>
#include "SDLlibs.h"
#include "main.h"
#include "classes.h"
#include <string>
#include <cmath>
#include <list>
#include <time.h>

#define SAMPLERATE 44100

using namespace std;

const int WIDTH = 900;
const int HEIGHT = 900;

const int N = 10;
const int offset = 150;
const int offsety = 50;
const int L = 300;
const int rad = 12;
const int start = 30;
const double alpha = 70*M_PI/180; // does work for other angles :)

class Ball{
	public:
	double R, theta;
	double w;
	uint32_t cycle, rem, bounces, target;
	uint8_t r,g,b;
	Mix_Chunk* audio;
	Ball(double R, double theta, double w, uint32_t cycle, uint32_t rem, uint8_t r, uint8_t g, uint8_t b, Mix_Chunk* audio){
		this->R=R;this->theta=theta;this->w=w;this->cycle=cycle;this->rem=rem;this->r=r;this->g=g;this->b=b;this->audio=audio;
		this->bounces = 0;
		target = cycle;
		if(rem != 0) target++;
	}
};

int gcd(int a, int b){
	int temp = a;
	if(a < b){
		a = b;
		b = temp;
	}
	while(a%b != 0){
		temp = b;
		b = a%b;
		a = temp;
	}
	return b;	
}

int lcm2(int a, int b){
	return (a*b)/gcd(a,b);
}

int lcm(int arr[], int count){
	if(count == 0) return -1;
	if(count == 1) return arr[0];
	int a = arr[0];
	for(int i=1;i<count;i++){
		a = lcm2(a,arr[i]);
	}
	return a;
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	SDL_Renderer* renderer;
	SDL_Window* window;
	//==================window===============
	uint32_t WINDOW_FLAGS = SDL_WINDOW_SHOWN; // | SDL_WINDOW_FULLSCREEN_DESKTOP;//SDL_WINDOW_BORDERLESS ;
	uint32_t SDL_FLAGS = SDL_INIT_VIDEO | SDL_INIT_TIMER;
	uint32_t IMG_FLAGS = IMG_INIT_PNG;

	INIT(&window, "my game", WIDTH, HEIGHT, &renderer, WINDOW_FLAGS, SDL_FLAGS, true, IMG_FLAGS);
	
	Load_Assets(renderer);

	//instantialize
        if(Mix_OpenAudio(SAMPLERATE, AUDIO_S8, 8, 1024)==-1) {
                printf("Mix_OpenAudio: %s\n", Mix_GetError());
                exit(2);
        }
	SDL_Delay(500);
	SDL_RaiseWindow(window);
	//=============local vars================
	
	SDL_Event* e = new SDL_Event;
	list<Ball*> balls;
	double len = L/sin(alpha/2);
	double step = len/N;
	double diff = rad/sin(alpha/2);
	uint8_t buff[N][SAMPLERATE/4];
	double base=0;
	double jmps[] =  /*{1}; 	{3,4,5};*/ 	{4,3,5}; 	/*{2,2,1,2,2,2,1};	{2,1,2,2,2,1,2};*/
	//		chromatic	minor arp	major arp	 major scale		  minor scale
	int jmplen = sizeof(jmps)/sizeof(double);
	int T = 20000;
	cout<<T<<endl;
	for(int i=0;i<N;i++){
		for(int j=0;j<SAMPLERATE/4;j++){
			buff[i][j] = (127/N)*sin((double)j*2*M_PI*55*pow(2,base/12)/SAMPLERATE);
		}
		base+=jmps[i%jmplen];
		Ball* ball= new Ball(offset + i*step,-1*alpha/2, (start+i)*alpha/T,(int)(T/(start+i)),T-((start+i)*(T/(start+i))), rand()%255, rand()%255, rand()%255, Mix_QuickLoad_RAW(buff[i], SAMPLERATE/4));
		cout<<start+i<<'\t'<<ball->cycle<<'\t'<<ball->rem<<'\t'<<ball->w<<endl;
		balls.push_back(ball);
	}
	//=============main cycle================
	list<Ball*>::iterator bbeg = balls.begin();
	list<Ball*>::iterator bend = balls.end();
	list<Ball*>::iterator itt;
	double slope = 1/tan(alpha/2);
	int framecount =0;
	while(e->type != SDL_QUIT){
		SDL_PollEvent(e);
		//control

		itt = bbeg;
		while(itt!=bend){
			if(framecount!=0 && framecount == (*itt)->target ){
				if((*itt)->bounces%2 == 0) (*itt)->theta = alpha/2;
				else (*itt)->theta = -alpha/2;
				(*itt)->target += (*itt)->cycle;
				if((*itt)->bounces < (*itt)->rem) (*itt)->target++;
			} // manual fix to the bug caused by floating point error
			if(((*itt)->theta >= alpha/2 && (*itt)->w >0 ) || ((*itt)->theta <= -alpha/2 && (*itt)->w <0)){
				(*itt)->w *= -1;
				(*itt)->bounces++;
				Mix_PlayChannel(-1,(*itt)->audio,0);
			}
			(*itt)->theta += (*itt)->w;
		//	cout<<(*itt)->w<<endl;
			itt++;
		}
	
		//draw
		SDL_SetRenderDrawColor(renderer, 255,255,255,255);
		SDL_RenderClear(renderer);
	

		SDL_SetRenderDrawColor(renderer,0,0,0,255);	
		SDL_RenderDrawLine(renderer, WIDTH/2, HEIGHT - offsety+diff, WIDTH/2-offset*sin(alpha/2)-L, HEIGHT-offsety-(int)((offset*sin(alpha/2)+L)*slope)+diff);
		SDL_RenderDrawLine(renderer, WIDTH/2, HEIGHT - offsety+diff, WIDTH/2+offset*sin(alpha/2)+L, HEIGHT-offsety-(int)((offset*sin(alpha/2)+L)*slope)+diff);


		itt = bbeg;
		while(itt!=bend){
			SDL_SetRenderDrawColor(renderer, (*itt)->r, (*itt)->g, (*itt)->b, 255); 
			my_RenderFillEllipse(renderer, WIDTH/2+(*itt)->R*sin((*itt)->theta), HEIGHT-offsety - (*itt)->R*cos((*itt)->theta), rad);
			itt++;
		}
		
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
		framecount++;

	}
	//==============clean up=================
	balls.clear();
	//delete objects

	Unload_Assets();
	//================Exit===================
	Mix_CloseAudio();
	QUIT(renderer, window, true);
	return 0;
}
