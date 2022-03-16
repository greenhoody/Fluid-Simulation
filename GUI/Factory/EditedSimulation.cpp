#include "EditedSimulation.h"
#include <corecrt_malloc.h>
#include <string.h>
#include <algorithm>
#include"pch.h"

#define IX(i,j) ((i)+(size+2)*(j))

EditedSimulation::EditedSimulation(int size, float diffiusion, float viscosity, float dt) {
	this->size = size;
	this->diff = diffiusion;
	this->visc = viscosity;
	this->dt = dt;

	this->u = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->u_prev = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->v = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->v_prev = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->dens = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->dens_prev = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->source = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->walls = (bool*)calloc((size + 4) * (size + 4), sizeof(bool));

	for (int i = 0; i < size + 4; i++) {
		walls[IX(0, i)] = true;
		walls[IX(1, i)] = true;
		walls[IX(size+2, i)] = true;
		walls[IX(size+3, i)] = true;
	}

	for (int i = 0; i < size + 4; i++) {
		walls[IX(i, 0)] = true;
		walls[IX(i, 1,)] = true;
		walls[IX(i, size + 2)] = true;
		walls[IX(i, size + 3)] = true;
	}
}

EditedSimulation::~EditedSimulation() {
	free(u);
	free(u_prev);
	free(v);
	free(v_prev);
	free(dens);
	free(dens_prev);
	free(source);
	free(walls);
}

void EditedSimulation::NextFrame(float* copy_array) {
	vel_step(u, v, u_prev, v_prev, visc);
	dens_step(dens, dens_prev, u, v, diff);
	memcpy(copy_array, dens, sizeof(float) * (size + 2) * (size + 2));
}

void EditedSimulation::AddDensity(int x, int y, float density) {
	int index = IX(x + 1, y + 1);
	dens[index] += density;
	if (dens[index] > 1) {
		dens[index] = 1;
	}
}

void EditedSimulation::AddVelocity(int x, int y, float h_velocity, float v_velocity) {
	int index = IX(x + 1, y + 1);
	v_prev[index] += v_velocity;
	u_prev[index] += h_velocity;
}

void EditedSimulation::AddWall(int x, int y) {
	walls[IX(x + 1, y + 1)] = true;
}

void EditedSimulation::DeleteWall(int x, int y) {
	walls[IX(x + 1, y + 1)] = false;
}

void EditedSimulation::AddSource(int x, int y, float volume) {
	source[IX(x + 1, y + 1)] = volume;
}

void EditedSimulation::DeleteSource(int x, int y) {
	source[IX(x + 1, y + 1)] = 0;
}

//========================================================================================

void EditedSimulation::set_bnd(int b, float* x) {
	for (int i = 1; i < this->size + 3; i++) {
		for (int j = 1; i < this->size + 3; i++) {
			// b1 horyzontalnie b2 pionowo
			x[IX(i - 1, j - 1)] = (b == 2 && walls[IX(i, j)] && walls[IX(i, j - 1)]) ? -x[IX(i-1, j - 2)] : x[IX(i - 1, j  -2)]; // gora
			x[IX(i - 1, j - 1)] = (b == 2 && walls[IX(i, j)] && walls[IX(i, j + 1)]) ? -x[IX(i - 1, j)] : x[IX(i - 1, j)];//dol
			x[IX(i - 1, j - 1)] = (b == 1 && walls[IX(i, j)] && walls[IX(i - 1, j)]) ? -x[IX(i - 2, j -1)] : x[IX(i - 2, j -1)];//lewo
			x[IX(i - 1, j - 1)] = (b == 1 && walls[IX(i, j)] && walls[IX(i + 1, j)]) ? -x[IX(i, j - 1)] : x[IX(i, j - 1)];//prawo
		}
	}

	x[IX(0, 0)] = 0.5 * (x[IX(1, 0)] + x[IX(0, 1)]); //lewygorny
	x[IX(0, size + 1)] = 0.5 * (x[IX(1, size + 1)] + x[IX(0, size)]); //lewydolny
	x[IX(size + 1, 0)] = 0.5 * (x[IX(size + 1, 1)] + x[IX(size, 0)]); //prawygorny
	x[IX(size + 1, size + 1)] = 0.5 * (x[IX(size, size + 1)] + x[IX(size + 1, size)]); //prawydolny
}

void EditedSimulation::diffuse(int b, float* x, float* x0, float diff) {
	float a = dt * diff * size * size;

	for (int k = 0; k < 20; k++) {
		for (int i = 1; i < size + 1; i++) {
			for (int j = 1; j < size + 1; j++) {
				x[IX(j, i)] = (x0[IX(j, i)] + a * (x[IX(j - 1, i)] + x[IX(j + 1, i)] +
					x[IX(j, i - 1)] + x[IX(j, i + 1)])) / (1 + 4 * a);
			}
		}
		set_bnd(b, x);
	}
}

void EditedSimulation::advect(int  b, float* d, float* d0, float* u, float* v) {
	int i, j, i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;
	dt0 = dt * (float)size;
	for (i = 1; i <= size; i++) {
		for (j = 1; j <= size; j++) {
			x = (float)i - dt0 * u[IX(i, j)];
			y = (float)j - dt0 * v[IX(i, j)];
			if (x < 0.5) x = 0.5f; if (x > size + 0.5) x = size + 0.5f;
			i0 = (int)x;
			i1 = i0 + 1;
			if (y < 0.5) y = 0.5f; if (y > size + 0.5) y = size + 0.5f;
			j0 = (int)y;
			j1 = j0 + 1;
			s1 = x - i0;
			s0 = 1 - s1;
			t1 = y - j0;
			t0 = 1 - t1;
			d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
				s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
		}
	}
	set_bnd(b, d);
}

void EditedSimulation::project(float* u, float* v, float* p, float* div) {
	int i, j, k;
	float h;
	h = 1.0f / size;
	for (i = 1; i <= size; i++) {
		for (j = 1; j <= size; j++) {
			div[IX(i, j)] = -0.5f * h * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
				v[IX(i, j + 1)] - v[IX(i, j - 1)]);
			p[IX(i, j)] = 0;
		}
	}
	set_bnd(0, div);
	set_bnd(0, p);

	for (k = 0; k < 20; k++) {
		for (i = 1; i <= size; i++) {
			for (j = 1; j <= size; j++) {
				p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
					p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
			}
		}
		set_bnd(0, p);
	}

	for (i = 1; i <= size; i++) {
		for (j = 1; j <= size; j++) {
			u[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
			v[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
		}
	}

	set_bnd(1, u);
	set_bnd(2, v);
}

void EditedSimulation::vel_step(float* u, float* v, float* u0, float* v0, float visc) {
	
	add_source(u, u0); add_source(v, v0);
	diffuse(1, u0, u, visc);
	diffuse(2, v0, v, visc);
	project(u0, v0, u, v);
	advect(1, u, u0, u0, v0);
	advect(2, v, v0, u0, v0);
	project(u, v, u0, v0);
}

void EditedSimulation::dens_step(float* x, float* x0, float* u, float* v, float diff) {
	//add_source(x0, source);
	diffuse(0, x0, x, diff);
	advect(0, x, x0, u, v);
}

void EditedSimulation::add_source(float* x, float* s) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			x[IX(i, j)] += s[IX(i, j)]*dt;
		}
	}
}