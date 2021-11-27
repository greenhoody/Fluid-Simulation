#include "pch.h"
#include "Simulator.h"
#include <stdlib.h>


//TODO indeksy s¹ Ÿle

//i - kolumna czyli OX, j - rz¹d cyli OY
#define IX(i,j) ((i)+(width2)*(j))
#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}

float* Simulator::NextFrame(float dt) {
	vel_step(hVelocity, vVelocity, vPrev, hPrev, viscosity, dt);
	dens_step(dens, densPrev, hVelocity, vVelocity, diffusion, dt);
	return dens;
}

// TODO magic with forces and project
void Simulator::vel_step(float* h, float* v, float* h0, float* v0, float visc, float dt) {
	source(h, h0, dt);
	source(v, v0, dt);
	SWAP(h0, h);
	diffuse(1, h, h0, visc, dt);
	SWAP(v0, v);
	diffuse(2, v, v0, visc, dt);
	project(h, v, h0, v0);
	SWAP(h0, h);
	SWAP(v0, v);
	advect(1, h, h0, h0, v0, dt);
	advect(2, v, v0, h0, v0, dt);
	project(h, v, h0, v0);
}

void Simulator::dens_step(float* x, float* x0, float* h, float* v, float diff, float dt) {
	source(x, x0, dt);
	SWAP(x0, x);
	diffuse(0,x, x0, diff, dt);
	SWAP(x0, x);
	advect(0, x, x0, h, v, dt);

}

// tutaj tak samo jak advect mo¿e popsuæ siê z tego powodu ¿e nie jest kwadratem albo uY i uX musza byc takie same
void Simulator::project(float* h, float* v, float* h0, float* v0) {

	float uY = 1.0 / height;
	float uX = 1.0 / width;

	for (int i = 2; i <= (width + 2); i++) {
		for (int j = 2; j <= (height + 2); j++) {
			v0[IX(i,j)] = -0.5 * uY *(h[IX(i + 1, j)] - h[IX(i - 1, j)]+v[IX(i, j + 1)] - v[IX(i, j - 1)]);
			h0[IX(i, j)] = 0;
		}
	}
	bnd(0, v0);
	bnd(0, h0);

	for (int k = 0; k < 20; k++) {
		for (int i = 2; i <= (width + 2); i++) {
			for (int j = 2; j <= (height + 2); j++) {
				h0[IX(i, j)] = (v0[IX(i, j)] + h0[IX(i - 1, j)] + h0[IX(i + 1, j)] + h0[IX(i, j - 1)] + h0[IX(i, j + 1)]) / 4;
			}
		}
		bnd(0, h0);
	}


	for (int i = 2; i <= (width + 2); i++) {
		for (int j = 2; j <= (height + 2); j++) {
			h[IX(i, j)] -= 0.5 * (h0[IX(i + 1, j)] - h0[IX(i - 1, j)]) / uX;
			v[IX(i, j)] -= 0.5 * (h0[IX(i, j + 1)] - h0[IX(i, j - 1)]) / uY;
		}
	}
	bnd(1, h);
	bnd(2, v);
}

// gdyby symulacja by³a zeskalowana/zniekszta³cona w poziomie lub pionie znaczy, ¿e musi byæ kwadratem przez t¹ funkcjê, albo zamist oddzielnych dtX dtY u¿yæ tylko jednej wartosci zapewne tej mniejszej
void Simulator::advect(int b, float* grid, float* grid2, float* h, float* v, float dt) {
	float dtX = dt * width;
	float dtY = dt * height;

	for (int i = 2; i <= width; i++) {
		for (int j = 2; j <= height; j++) {
			float x = i - dtX * h[IX(i, j)];
			float y = i - dtY * v[IX(i, j)];
			//i tutaj mo¿e siê wiele rzeczy popsuc, nawet bardzo duzo
			// przesuno³em o jedno
			if (x < 1.5)
				x = 1.5;
			// 2 lewej bariery + 0.5 prawej
			else if (x > width + 2.5)
				x = width + 2.5;

			int i0 = (int)x;
			int i1 = i0 + 1;

			if (y < 1.5)
				y = 0.5;
			else if (y > height + 2.5)
				y = height + 2.5;

			int j0 = (int)y;
			int j1 = j0 + 1;

			float s1 = x - i0;
			float s0 = 1 - s1;
			float t1 = y - j0;
			float t0 = 1 - t1;

			//   d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)]+t1*d0[IX(i0,j1)])+ s1* (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
			grid[IX(i, j)] = s0 * (t0 * grid2[IX(i0, j0)] + t1 * grid2[IX(i0, j1)]) + s1 * (t0 * grid2[IX(i1, j0)] + t1 * grid2[IX(i1, j1)]);

		}
	}
	bnd(b, grid);
}

void Simulator::diffuse(int b, float* grid, float* grid2, float diff, float dt) {
	float a = dt * diff * height * width;

	for (int k = 0; k < 20; k++) {
		for (int i = 2; i <= height; i++) {
			for (int j = 2; j <= width; j++) {
				grid[IX(i, j)] = (grid2[IX(i, j)] + a * (grid[IX(i - 1, j)] + grid[IX(i + 1, j)] +
					grid[IX(i, j - 1)] + grid[IX(i, j + 1)])) / (1 + 4 * a);
			}
		}
		bnd(b, grid);
	}
}

void Simulator::bnd(int b, float* x)
{
//orginal tylko krawedzie
//for (i = 1; i <= N; i++) {
//	x[IX(0, i)] = b == 1 ? –x[IX(1, i)] : x[IX(1, i)];
//	x[IX(N + 1, i)] = b == 1 ? –x[IX(N, i)] : x[IX(N, i)];
//	x[IX(i, 0)] = b == 2 ? –x[IX(i, 1)] : x[IX(i, 1)];
//	x[IX(i, N + 1)] = b == 2 ? –x[IX(i, N)] : x[IX(i, N)];
//}

//sprawdza bariery rownie¿ w srodku, musz¹ byæ one szerokie i wysokie na co najmnie 2 pola
	for (int i = 1; i < this->width2; i++) {
		for (int j = 1; i < this->height2; i++) {
			// b1 horyzontalnie b2 pionowo
			x[IX(i, j)] = (b == 2 && boundaries[IX(i, j)] && boundaries[IX(i, j - 1)]) ? -x[IX(i, j - 1)] : x[IX(i, j - 1)]; // gora
			x[IX(i, j)] = (b == 2 && boundaries[IX(i, j)] && boundaries[IX(i, j + 1)]) ? -x[IX(i, j + 1)] : x[IX(i, j + 1)];//dol
			x[IX(i, j)] = (b == 1 && boundaries[IX(i, j)] && boundaries[IX(i - 1, j)]) ? -x[IX(i - 1, j)] : x[IX(i - 1, j)];//lewo
			x[IX(i, j)] = (b == 1 && boundaries[IX(i, j)] && boundaries[IX(i + 1, j)]) ? -x[IX(i + 1, j)] : x[IX(i + 1, j)];//prawo
		}
	}


// orginalne od naroznikow od naroznikow
//x[IX(0, 0)] = 0.5 * (x[IX(1, 0)] + x[IX(0, 1)]);
//x[IX(0, N + 1)] = 0.5 * (x[IX(1, N + 1)] + x[IX(0, N)]);
//x[IX(N + 1, 0)] = 0.5 * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
//x[IX(N + 1, N + 1)] = 0.5 * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);

	x[IX(1, 1)] = 0.5 * (x[IX(2, 1)] + x[IX(1, 2)]); //lewygorny
	x[IX(1, height + 1)] = 0.5 * (x[IX(1, height)] + x[IX(2, height + 1)]); //lewydolny
	x[IX(width + 1, 1)] = 0.5 * (x[IX(width , 1)] + x[IX(width + 1, 2)]); //prawygorny
	x[IX(width + 1, height + 1)] = 0.5 * (x[IX(width, height + 1)] + x[IX(width + 1, height)]); //prawydolny
}

void Simulator::source(float* grid, float* sources, float dt) {
	for (int i = 0; i < this->size; i++) {
		grid[i] += dt * sources[i];
	}
}

Simulator::Simulator(int height, int widht, float viscosity, float diffusion) {
	this->viscosity = viscosity;
	this->diffusion = diffusion;
	this->height = height;
	this->width = width;
	height2 = height + 4;
	width2 = width + 4;
	size = height2 * width2;
	hVelocity = (float*) calloc(size, sizeof(float));
	hPrev = (float*) calloc(size, sizeof(float));
	vVelocity = (float*) calloc(size, sizeof(float));
	vPrev = (float*) calloc(size, sizeof(float));
	dens = (float*) calloc(size, sizeof(float));
	densPrev = (float*) calloc(size, sizeof(float));
	densSources = (float*) calloc(size, sizeof(float));
	boundaries = (bool*) calloc(size, sizeof(bool));
	
	for (int i = 0; i < height2; i++) {
		boundaries[IX(0,i)] = true;
		boundaries[IX(1, i)] = true;
		boundaries[IX(width2 - 1,i)] = true;
		boundaries[IX(width2,i)] = true;
	}
	for (int i = 0; i < width2; i++) {
		boundaries[IX(i, 0)] = true;
		boundaries[IX(i, 1)] = true;
		boundaries[IX(i, height2 - 1)] = true;
		boundaries[IX(i, height2)] = true;
	}
}