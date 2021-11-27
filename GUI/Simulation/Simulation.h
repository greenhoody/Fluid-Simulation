#pragma once
#include "Simulator.h"


class Simulation 
{
private:
	Simulator *simulator;

public:
	float* GetNextFrame(float dt);
	void AddForce(int x, int y, float dx, float dy);
	void AddBoundaries(int x1, int y1, int x2, int y2);
	void DeleteBoundaries(int x1, int y1, int x2, int y2);
	void AddSource(int x, int y, float s);


	// constructor 
	Simulation(int height, int width, float viscosity,float diffusion);
};