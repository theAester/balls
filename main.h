void INIT(SDL_Window** window,
		const char* window_name, 
		uint32_t window_width, 
		uint32_t window_height, 
		SDL_Renderer** renderer, 
		uint32_t WINDOW_FLAGS, 
		uint32_t SDL_FLAGS,
		bool image = false,
		uint32_t IMG_FLAGS = 0){
	SDL_Init(SDL_FLAGS);
	*window = SDL_CreateWindow(window_name, 
				  SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED, 
				  window_width, 
				  window_height,WINDOW_FLAGS);

	*renderer = SDL_CreateRenderer(*window, -1, 0);
	if(image) IMG_Init(IMG_FLAGS);
	return;
}

void QUIT(SDL_Renderer* renderer, SDL_Window* window, bool image = false){
	SDL_DestroyWindow( window );
	SDL_DestroyRenderer( renderer );
	if(image) IMG_Quit();
	SDL_Quit();
	return;
}

struct my_Rect : SDL_Rect{
	my_Rect(int x, int y, int width, int height){
		this->x = x;
		this->y = y;
		this->w = width;
		this->h = height;
	}
};


void my_RenderFillEllipse(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	const double sqrt2rec = 0.707106781; 
	const int adjlen = (int) radius*sqrt2rec;

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle

		SDL_RenderDrawLine(renderer, centreX+x,centreY-y, centreX+x,centreY+y);
		SDL_RenderDrawLine(renderer, centreX-x,centreY-y, centreX-x, centreY+y);
		SDL_RenderDrawLine(renderer, centreX+y, centreY-x, centreX+y, centreY+x);
		SDL_RenderDrawLine(renderer, centreX-y, centreY-x, centreX-y, centreY+x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
	my_Rect* rect = new my_Rect(centreX-adjlen, centreY-adjlen, 2*adjlen, 2*adjlen);
	SDL_RenderFillRect(renderer,rect);
	delete rect;
}
