#include <stdlib.h>

#include "Simulator.h"

#define IX(i,j) ((i)+(width2+2)*(j))

void Simulator::vel_step(float visc, float dt) {
	source(dt);

}

// TODO
void Simulator::bnd() {
	// for the ones
	for (int i = 0; i < width2; i++) {
		boundaries[i] = true;
		boundaries[size - i - 1] = true;
	}

	for (int i = 0; i < height2; i++) {
		boundaries[i * width2] = true;
		boundaries[(i + 1) * width2 - 1] = true;
	}
}
// TODO:: potrzebne bnd
void Simulator::diffuse(int heigth, int width, int b, float* x, float* x0, float diff, float dt) {
	float a = dt * diff * heigth * width;

	for (int k = 0; k < 20; k++) {
		for (int i = 1; i <= heigth; i++) {
			for (int j = 1; j <= width; j++) {
				x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
					x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
			}
		}
//		bnd(N, b, x);
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