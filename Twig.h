#ifndef TWIG
#define TWIG

#include <random>
#include <SDL.h>
using namespace std;

class Twig
{
public:
	double sx;
	double sy;
	double phi;
	double length;
	int colour;
	double randm();
	default_random_engine* gen;
	exponential_distribution<double>* exdist;

	bool blocked;
	Twig(double ix, double iy, double iphi, double len, int col, default_random_engine* gen, exponential_distribution<double>* exdist);
	~Twig();
	void grow(int tick, vector<Twig>* twigList, vector<Twig>* children);
	bool collidePoint(double x, double y);
	void draw(SDL_Renderer* renderer);
	bool operator==(Twig t);

};




#endif