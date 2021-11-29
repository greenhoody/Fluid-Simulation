#pragma once
#include "Simulator.h"
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;


class Simulation 
{
private:
	//Simulator *simulator;

public:
	void GetNextFrame(float* density, float dt);
	void AddForce(int x, int y, float dx, float dy);
	void AddBoundaries(int x1, int y1, int x2, int y2);
	void DeleteBoundaries(int x1, int y1, int x2, int y2);
	void AddSource(int x, int y, float s);


	// constructor 
	Simulation(int height, int width, float viscosity,float diffusion);

	//po tescie zabrac zabraæ
	Simulator* simulator;
};