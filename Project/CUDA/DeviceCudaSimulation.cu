﻿#include <cuda_runtime.h>
#include "DeviceCudaSimulation.cuh"
#include <cooperative_groups.h>
#include <cuda_runtime_api.h> 
#include <cuda.h>

#define IX(i,j) ((i)+(N+2)*(j))


// wątek zero rogi, reszta krawędzie, wiem że nie optymalne, ale może późjniej nad tym pomyśle i wywali się przy zamałej ilości wątków, na początku synchronizacja grid. Ta funkcja jest wywołyuwana wiele razy, więc wiele synchronizacji więc trzeba sprawdzić póxniej czy potrzebne
__device__ void set_bnd(int N, int b, float* x)
{
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	
	cooperative_groups::grid_group g = cooperative_groups::this_grid();
	g.sync();

	if (index >= 1 && index <= N) {
		x[IX(0, index)] = b == 1 ? -x[IX(1, index)] : x[IX(1, index)];
		x[IX(N + 1, index)] = b == 1 ? -x[IX(N, index)] : x[IX(N, index)];
		x[IX(index, 0)] = b == 2 ? -x[IX(index, 1)] : x[IX(index, 1)];
		x[IX(index, N + 1)] = b == 2 ? -x[IX(index, N)] : x[IX(index, N)];
	}

	else if (index == 0)
	{
		x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
		x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
		x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
		x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
	}
	g.sync();

}

__device__ void diffuse(int N, int b, float* x, float* x0, float diff, float dt)
{
	int n = N * N;
	float a = dt * diff * n;
	
	
	int index = 0;
	int cores = gridDim.x * blockDim.x;
	cooperative_groups::grid_group g = cooperative_groups::this_grid();
	
	//kompiluje się czyli intellisense nie ogarnia

	for (int k = 0; k < 20; k++) {
		
		// wykonanie przypadających komurek
		index = blockIdx.x * blockDim.x + threadIdx.x;
		while (index < n) {
			// +1 po od 1 zaczynały się pętle, a nie od zera
		int j = (index / N) + 1;
		int i = (index % N) + 1;

			x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
				x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + (4 * a));
			index += cores;
		}
		g.sync();
		set_bnd(N, b, x);
	}
}

__device__ void advect(int N, int b, float* d, float* d0, float* u, float* v, float dt)
{
	cooperative_groups::grid_group g = cooperative_groups::this_grid();
	int n = N * N;
	int cores = gridDim.x * blockDim.x;
	int index = blockIdx.x * blockDim.x + threadIdx.x;

	while(index < n)
	{
		// +1 po od 1 zaczynały się pętle, a nie od zera
		int j = (index / N) + 1;
		int i = (index % N) + 1;


		int i0, j0, i1, j1;
		float x, y, s0, t0, s1, t1, dt0;
		dt0 = dt * (float)N;

		x = (float)i - dt0 * u[IX(i, j)];
		if (x < 0.5) x = 0.5f;
		if (x > N + 0.5) x = N + 0.5f;
		i0 = (int)x;
		i1 = i0 + 1;

		//proporcje ile gę┌stości wylądowało z których komórek
		s1 = x - (float)i0;
		s0 = 1.0f - s1;

		y = (float)j - dt0 * v[IX(i, j)];
		if (y < 0.5) y = 0.5f;
		if (y > N + 0.5) y = N + 0.5f;
		j0 = (int)y;
		j1 = j0 + 1;

		//proporcje ile gęstości wylądowało z których komórek
		t1 = y - (float)j0;
		t0 = 1.0f - t1;

		d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
			s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);

		index += cores;
	}
	g.sync();
	set_bnd(N, b, d);
}

__device__ void project(int N, float* u, float* v, float* p, float* div)
{
	cooperative_groups::grid_group g = cooperative_groups::this_grid();
	int n = N * N;
	int cores = blockDim.x * gridDim.x;

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	// +1 po od 1 zaczynały się pętle, a nie od zera
	float h = 1.0f / N;

	while (index < n) {
		int j = (index / N) + 1;
		int i = (index % N) + 1;


		div[IX(i, j)] = -0.5f * h * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
			v[IX(i, j + 1)] - v[IX(i, j - 1)]);
		p[IX(i, j)] = 0.0f;

		index += cores;
	 }
	g.sync();
	set_bnd(N, 0, div);
	g.sync();
	set_bnd(N, 0, p);

	
	for (int k = 0; k < 20; k++) {
		index = blockIdx.x * blockDim.x + threadIdx.x;
		while (index < n)
		{
		int j = (index / N) + 1;
		int i = (index % N) + 1;

			p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
				p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
			index += cores;

		}
		g.sync();
		set_bnd(N, 0, p);
	}

	index = blockIdx.x * blockDim.x + threadIdx.x;
	while (index < n)
	{
		int j = (index / N) + 1;
		int i = (index % N) + 1;

		u[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
		v[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
		index += cores;
	}
	g.sync();
	set_bnd(N, 1, u);
	g.sync();
	set_bnd(N, 2, v);
}

__global__ void cuda_NextFrame(int N, float* d_dens, float* d_dens_prev, float* d_u, float* d_v, float* d_u_prev, float* d_v_prev, float visc, float diff , float dt) 
{

	diffuse(N, 1, d_u_prev, d_u, visc, dt);
	diffuse(N, 2, d_v_prev, d_v, visc, dt);
	project(N, d_u_prev, d_v_prev, d_u, d_v);
	advect(N, 1, d_u, d_u_prev, d_u_prev, d_v_prev, dt);
	advect(N, 2, d_v, d_v_prev, d_u_prev, d_v_prev, dt);
	project(N, d_u, d_v, d_u_prev, d_v_prev);

	diffuse(N, 0, d_dens_prev, d_dens, diff, dt);
	advect(N, 0, d_dens, d_dens_prev, d_u, d_v, dt);
}

__global__ void addDensity(int N, float* d_dens, int x1, int x2, int y1, int y2, float dens)
{
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int cores = blockDim.x * gridDim.x;
	int deltay = y2 - y1;
	int deltax = x2 - x1;
	int n = deltay * deltax;


	while (index < n) 
	{
		int j = (index / deltax);
		int i = (index % deltax);

		d_dens[IX(x1 + i, y1 + j)] += dens;

		if (d_dens[IX(x1 + i, y1 + j)] > 1)
		{
			d_dens[IX(x1 + i, y1 + j)] = 1;
		}
		index += cores;
	}
}

__global__ void addVelocity(int N, float* d_u, float* d_v, int x, int y, int r, float u_velocity, float v_velocity)
{
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int cores = blockDim.x * gridDim.x;
	int j = (index / (r*2));
	int i = (index % (r*2));
	int r_square = r * r;
	int n = r_square * 4;
	x -= r;
	y -= r;

	while (index < n) {

		if (N > x + i && x + i > 0 && N > y + j && y + j > 0 && i * i + j * j <= r_square)
		{
			d_u[IX(x + i, y + j)] += u_velocity;
			d_v[IX(x + i, y + j)] += v_velocity;
		}
		index += cores;
	}
}