#include "Twig.h"
#include <vector>
#include <random>
#include <cmath>
#include <SDL.h>
#include <iostream>
using namespace std;

Twig::Twig(double ix, double iy, double iphi, double len, int col, default_random_engine* igen, exponential_distribution<double>* iexdist)
{
	sx = ix;
	sy = iy;
	phi = iphi;
	length = len;
	colour = col;
	blocked = false;
	gen = igen;
	exdist = iexdist;
}

bool Twig::operator==(Twig t)
{
	return (t.sx == sx && t.sy == sy && t.length == length && t.phi == phi);
}

Twig::~Twig()
{

}

double d(double sx, double sy, double ex, double ey)
{
	//return (ex - sx)*(ex - sx) + (ey - sy)*(ey - sy);
	return sqrt((ex - sx)*(ex - sx) + (ey - sy)*(ey - sy));
}

double Twig::randm()
{
	return (*exdist)(*gen) - (*exdist)(*gen);
}

bool Twig::collidePoint(double x, double y)
{
	double threshold = 0.1;
	double endx = sx + length*sin(phi);
	double endy = sy + length*cos(phi);

	if (d(x, y, sx, sy) + d(endx, endy, x, y) - d(sx, sy, endx, endy) < threshold && d(x, y, sx, sy) + d(endx, endy, x, y) - d(sx, sy, endx, endy) > -threshold)
		return true;

	return false;
	/*
	double dxc = x - sx;
	double dyc = y - sy;

	double dxl = endx - sx;
	double dyl = endy - sy;

	double cross = dxc * dyl - dyc * dxl;

	if (abs(cross) > threshold)
		return false;
	if (abs(dxl) >= abs(dyl))
		return dxl > 0 ?
		sx <= x && x <= endx :
		endx <= x && x <= sx;
	else
		return dyl > 0 ?
		sy <= y && y <= endy :
		endy <= y && y <= sy;
	*/
}

void Twig::grow(int tick, vector<Twig> *twigList, vector<Twig>* children)
{
	if (blocked)
	{
		return;
	}
	
	double endx = sx + (length + 1)*sin(phi);
	double endy = sy + (length + 1)*cos(phi);

	//should only test twigs that are close to end, in radius that covers all twiglengths with probability 0.99 or something...
	//check twigs whose start or end is within meanmaxdist of my end

	for (int i = 0; i < twigList->size(); i++)
	{
		Twig ctwig = (*twigList)[i];
		if (ctwig.sx == sx && ctwig.sy == sy && ctwig.length == length && ctwig.colour == colour)
			continue;
		//for lambda=5, mean twiglenght is over 7
		double thr = 7*5;
		if (!((ctwig.sx - endx < thr && ctwig.sx - endx > -thr) && (ctwig.sy - endy < thr && ctwig.sy - endy > -thr)))
			continue;
		
		if (ctwig.collidePoint(endx, endy))
		{
			blocked = true;
			return;
		}
	}
	//No block, grow!
	length = length + 1;
	
	//sometimes, make new twigs at end
	double r = (*exdist)(*gen);
	if (r > 4)
	{
		Twig child1 = Twig(endx, endy, phi  + (randm()) / 20, 1, colour + (int)((*exdist)(*gen) / 2), gen, exdist);
		Twig child2 = Twig(endx, endy, phi  + (randm()) / 20, 1, colour + (int)((*exdist)(*gen) / 2), gen, exdist);
		//twigList->push_back(child1);
		//twigList->push_back(child2);
		children->push_back(child1);
		children->push_back(child2);
		blocked = true;
	}
}

void Twig::draw(SDL_Renderer* renderer)
{
	double endx = sx + length*sin(phi);
	double endy = sy + length*cos(phi);
	//map color to [1,16777216]
	//double c = colour / (1 + colour / 16777216);
	//double rr = 10 * c/256/256;
	//double gg = 10 * c/256;
	//double bb = 10 * c;
	
	double rr = 0;
	double gg = 0;
	double bb = 0;


	double maxcol = 3*256/6;
	double maxcolthird = maxcol / 3;
	double c = colour;
	if (c < maxcol / 3)
	{
		rr = c;
		gg = 0;
		bb = 0;
	}
	else if (c > maxcol / 3 && c < maxcol / 3 * 2)
	{
		rr = maxcolthird;
		gg = c - maxcolthird;
		bb = 0;
	}
	else
	{
		rr = 255 - fmin(c-255*2,255);
		gg = maxcolthird - fmin(c - 255, 255);
		bb = c - 2 * maxcolthird;
	}
	
	int r = (int)(10 * rr) % 256;
	int g = (int)(10 * gg) % 256;
	int b = (int)(10 * bb) % 256;
	SDL_SetRenderDrawColor(renderer, r,g,b, 255);
	SDL_RenderDrawLine(renderer, (int)(sx), (int)(sy), (int)(endx), (int)(endy));

}

