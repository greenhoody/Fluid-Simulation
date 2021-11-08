#include "pch.h"
#include "Simulator.h"

#include <stdlib.h>

#define IX(i,j) ((i)+(width2+2)*(j))
#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}

// TODO what is diff and implement advect
void Simulator::dens_step( float diff, float dt) {
	source(dt);
	SWAP(dens_prev, dens);
	diffuse(diff, dt);
	SWAP(dens_prev, dens);
	advect(dt);

}
// TODO magic with forces
void Simulator::vel_step(float visc, float dt) {
	source(dt);
	SWAP(v_pref, v);
	SWAP(u_prev, u);
	diffuse(visc, dt);
}

// velocity = 0 if density go into wall, works with walls inside simulation, but not exactly like in paper but it can work
void Simulator::bnd() {
	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++) {
			//vertical
			if (boundaries[IX(i - 1, j)] && v[IX(i, j)] < 0)
				v[IX(i, j)] = 0;
			else if (boundaries[IX(i + 1, j)] && v[IX(i, j)] > 0)
				v[IX(i, j)] = 0;
			//horizontal
			if (boundaries[IX(i - 1, j)] && u[IX(i, j)] < 0)
				u[IX(i, j)] = 0;
			else if (boundaries[IX(i + 1, j)] && u[IX(i, j)] > 0)
				u[IX(i, j)] = 0;
		}
}


void Simulator::diffuse(float diff, float dt) {
	float a = dt * diff * height * width;

	for (int k = 0; k < 20; k++) {
		for (int i = 1; i <= height; i++) {
			for (int j = 1; j <= width; j++) {
				dens[IX(i, j)] = (dens_prev[IX(i, j)] + a * (dens[IX(i - 1, j)] + dens[IX(i + 1, j)] +
					dens[IX(i, j - 1)] + dens[IX(i, j + 1)])) / (1 + 4 * a);
			}
		}
		bnd();
	}
}


void Simulator::source(float dt) {
	for (int i = 0; i < size; i++) {
		dens[i] = dt * sources[i];
	}
}

void Simulator::Add_Source(int x, int y, float quantity) {
	sources[(y + 1) * width2 + (x + 1)] = quantity;
}

Simulator::Simulator(int height, int width) {
	this->height = height;
	this->width = width;
	height2 = height + 2;
	width2 = width + 2;
	size = (height + 2) * (width + 2);
	u = (float*) calloc(size, sizeof(float));
	u_prev = (float*) calloc(size, sizeof(float));
	v = (float*) calloc(size, sizeof(float));
	v_pref = (float*) calloc(size, sizeof(float));
	dens = (float*) calloc(size, sizeof(float));
	dens_prev = (float*) calloc(size, sizeof(float));
	sources = (float*) calloc(size, sizeof(float));
	boundaries = (bool*) calloc(size, sizeof(bool));
	
	for (int i = 0; i < width2; i++) {
		boundaries[i] = true;
		boundaries[size - i - 1] = true;
	}
	for (int i = 0; i < height2; i++) {
		boundaries[i * width2] = true;
		boundaries[(i+1) * width2 - 1] = true;
	}
}