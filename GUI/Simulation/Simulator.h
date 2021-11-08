#pragma once

class Simulator {
// 
public:
	float * u, * v, * u_prev, * v_pref, * dens, * dens_prev, * sources;
	bool* boundaries;

private:
	int size;
	int height, width;
	// height and widht of simulation with added boundaries
	int height2, width2;

// methods
public:
	Simulator(int height, int widht);
	void Add_Source(int x, int y, float quantity);

private:
	
	void bnd();
	void source(float dt);
	void diffuse(float diff, float dt);
	void advect(float dt);
	void dens_step(float diff, float dt);
	void vel_step(float visc, float dt);
	void project(int height, int widht, float* u, float* v, float* p, float* div);
};
