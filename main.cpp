#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include "Sim.h"



int main(int argc, char* argv[])
{
	
	Sim simulation = Sim(1024-256, 2048-512);
	simulation.run();
	
	return 0;
}