#ifndef SIM
#define SIM

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <cmath>
#include <random>

using namespace std;

class Sim
{
private:
	//Draw Resolution:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;

	default_random_engine gen;
	exponential_distribution<double> exdist;

	
public:
	Sim(int, int);
	~Sim();
	void run();
	bool running;
	bool initSDL();
	void close();
	SDL_Texture * loadTexture(std::string path);
};

#endif
