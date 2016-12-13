#include "Sim.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include "Twig.h"
#include <chrono>

using namespace std;

Sim::Sim(int h, int w)
{
	
	SCREEN_HEIGHT = h;
	SCREEN_WIDTH = w;

	
	running = false;

	if (!initSDL())
	{
		std::cout << "Could not init Sim." << std::endl;
	}


	gen = default_random_engine();
	exdist = exponential_distribution<double>(0.5);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	gen.seed(seed);

}

Sim::~Sim()
{
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Sim::run()
{
	vector<Twig> twigList;
	//vector<Twig> growList;
	Twig trunk = Twig((SCREEN_WIDTH/2), (int)(SCREEN_HEIGHT / 3*2), M_PI, 10, 1, &gen, &exdist);
	Twig trunk2 = Twig(600, 300, exdist(gen), 10, 100, &gen, &exdist);
	twigList.push_back(trunk);
	//growList.push_back(trunk);
	//twigList.push_back(trunk2);


	vector<int> growth;

	running = true;


	clock_t lastKey = clock();

	SDL_Event event;
	int counter = 0;
	while (running)
	{
		counter++;
		//Handle events on queue
		
		while (SDL_PollEvent(&event) != 0)
		{
			//User requests quit
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = event.button.x;
				int y = event.button.y;

			}
			if (event.type = SDL_KEYDOWN && (clock() - lastKey) > 300)
			{
				lastKey = clock();
				//switch (event.key.keysym.sym)
				//{
				//case SDLK_i:
				//	do stuff
				//	break;
				//case SDLK_u:
				//	do stuff
				//	break;
				//}
			}
		}
		
		//TICK START ---------------------------------------------------------------
		clock_t tickStart = clock();

		//int s = growList.size();
		int s = twigList.size();
		vector<Twig> children;
		for (int i = 0; i < s; i++)
		{
			//growList[i].grow(counter, &twigList, &children);
			twigList[i].grow(counter, &twigList, &children);
		}

		twigList.insert(twigList.end(), children.begin(), children.end());
		//growList.insert(growList.end(), children.begin(), children.end());
		/*
		for (int i = 0; i < growList.size(); i++)
		{
			Twig curr = growList[i];
			if (curr.blocked)
			{
				//growList.erase(find(growList.begin(), growList.end(), growList[i]));
			}
		}
		*/

		clock_t tickEnd = clock();
		if (counter%10 == 0)
			cout << "Tick time: " << tickEnd - tickStart << endl << twigList.size() << "       " << 1.0*(tickEnd - tickStart) / (twigList.size()) << endl;
		//TICK END -----------------------------------------------------------------
		
		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		
		//RENDER START -------------------------------------------------------------

		
		for (int i = 0; i < twigList.size(); i++)
		{
			twigList[i].draw(renderer);
		}
		

		//SDL_SetRenderDrawColor(renderer, 200, 255, 255, 255);
		//SDL_RenderDrawLine(renderer, 300, 300, 300 + (int)(80 * sin(counter)), 300 + (int)(80 * cos(counter)));
		//SDL_Rect rect = { y*CELL_WIDTH, x*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT };
		//SDL_SetRenderDrawColor(renderer, (r) % 256, (g) % 256, (b) % 255, 255);
		//SDL_RenderFillRect(renderer, &rect);

		//RENDER END ---------------------------------------------------------------

		
		//Update screen
		SDL_RenderPresent(renderer);

		//cout << "Number of Twigs: " << twigList.size() << endl;

		//calculate mean length
		/*
		double acc = 0;
		for (int i = 0; i < twigList.size(); i++)
		{
			acc += twigList[i].length;
		}
		acc = acc / twigList.size();
		cout << "Mean twig length = " << acc << endl;
		*/


		growth.push_back(twigList.size());

		if (counter % 20 == 0)
		{
			ofstream file;
			file.open("C:\\Users\\Michael\\Desktop\\growth.txt");
			for (int i = 0; i < growth.size(); i++)
			{
				file << growth[i] << ",";
			}
			file.close();
		}
		//SDL_Delay(10);
		
	}


}

bool Sim::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Error: SDL not initialized" << std::endl;
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Error: Linear Filtering not enabled" << std::endl;
	}
	window = SDL_CreateWindow("SDLTest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cout << "Error: Window not initialized." << std::endl;
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		std::cout << "Error: Renderer not initialized" << std::endl;
		return false;
	}
	
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout<< "Error: SDL_image not initialized." << std::endl;
		return false;
	}

	return true;
}

SDL_Texture * Sim::loadTexture(std::string path)
{
	SDL_Texture * newTex = NULL;
	SDL_Surface * loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Error: Surface not loaded." << std::endl;
		return NULL;
	}
	newTex = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTex == NULL)
	{
		std::cout << "Error: Texture not loaded." << std::endl;
		return NULL;
	}
	SDL_FreeSurface(loadedSurface);
	return newTex;
	//Don't forget to call SDL_DestroyTexture(*tex) 
}

void Sim::close()
{
	
}