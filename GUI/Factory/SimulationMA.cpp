#include "SimulationMA.h"
#include <corecrt_malloc.h>
#include <string.h>
#include <algorithm>
#include"pch.h"
#include <string>

#include <windows.h>
#include <debugapi.h>;


#define IX(i,j) ((i)+(size+2)*(j))

SimulationMA::SimulationMA(int size, float diffiusion, float viscosity, float dt) {
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
}

SimulationMA::~SimulationMA() {
	free(u);
	free(u_prev);
	free(v);
	free(v_prev);
	free(dens);
	free(dens_prev);
}

void SimulationMA::NextFrame(float* copy_array) {
	vel_step(size + 2, u, v, u_prev, v_prev, visc, dt);
	dens_step(size + 2, dens, dens_prev, u, v, diff, dt);
	memcpy(copy_array, dens, sizeof(float) * (size + 2) * (size + 2));
}

void SimulationMA::AddDensity(int x, int y, float density) {
	int index = IX(x + 1, y + 1);
	dens[index] += density;
	if (dens[index] > 1) {
		dens[index] = 1;
	}
}

void SimulationMA::AddVelocity(int x, int y, float h_velocity, float v_velocity) {
	int index = IX(x + 1, y + 1);
	//v z indeksem ujemnym wpisuje dane do u_prev/ mazanie po pamięci
	v[index] += v_velocity;
	u[index] += h_velocity;
}

//=====================================================================================================================

void SimulationMA::add_source(int N, float* x, float* s, float dt)
{
	int i, size = (N + 2) * (N + 2);
	for (i = 0; i < size; i++) x[i] += dt * s[i];
}

void SimulationMA::set_bnd(int b, float* x, int N)
{
	for (int i = 1; i < N - 1; i++) {
		x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
		x[IX(N - 1, i)] = b == 1 ? -x[IX(N - 2, i)] : x[IX(N - 2, i)];
		x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
		x[IX(i, N - 1)] = b == 2 ? -x[IX(i, N - 2)] : x[IX(i, N - 2)];
	}
	x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
	x[IX(0, N - 1)] = 0.5f * (x[IX(1, N - 1)] + x[IX(0, N - 2)]);
	x[IX(N - 1, 0)] = 0.5f * (x[IX(N-2, 0)] + x[IX(N - 1, 1)]);
	x[IX(N - 1, N - 1)] = 0.5f * (x[IX(N - 2, N - 1)] + x[IX(N - 1, N - 2)]);
}

void SimulationMA::diffuse(int b, float* x, float* x0, float diff, float dt, int iter, int N)
{
	float a = dt * diff * (N - 2) * (N - 2);
	lin_solve(b, x, x0, a, 1 + 4 * a, iter, N);
}


//jak na filmie
void SimulationMA::advect( int b, float* d, float* d0, float* u, float* v, float dt, int N)
{
	float i0, i1, j0, j1;

	float dtx = dt * (N - 2);
	float dty = dt * (N - 2);

	float s0, s1, t0, t1;
	float tmp1, tmp2, x, y;

	float Nfloat = N;
	float ifloat, jfloat;
	int i, j;

	for (j = 1, jfloat = 1; j < N - 1; j++, jfloat++) {
		for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++) {
			tmp1 = dtx * u[IX(i, j)];
			tmp2 = dty * v[IX(i, j)];
			x = ifloat - tmp1;
			y = jfloat - tmp2;

			if (x < 0.5f) x = 0.5f;
			if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
			i0 = floorf(x);
			i1 = i0 + 1.0f;
			if (y < 0.5f) y = 0.5f;
			if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
			j0 = floorf(y);
			j1 = j0 + 1.0f;

			s1 = x - i0;
			s0 = 1.0f - s1;
			t1 = y - j0;
			t0 = 1.0f - t1;

			int i0i = i0;
			int i1i = i1;
			int j0i = j0;
			int j1i = j1;

			d[IX(i, j)] =
				s0 * ((t0 * d0[IX(i0i, j0i)]) + (t1 * d0[IX(i0i, j1i)]))
				+ s1 * ((t0 * d0[IX(i1i, j0i)])+ (t1 * d0[IX(i1i, j1i)]));

		}
	}
	set_bnd(b, d, N);
}

void SimulationMA::project(float* u, float* v, float* p, float* div, int iter, int N)
{
	for (int j = 1; j < N - 1; j++) {
		for (int i = 1; i < N - 1; i++) {
			div[IX(i, j)] = -0.5f * (
				u[IX(i + 1, j)]
				- u[IX(i - 1, j)]
				+ v[IX(i, j + 1)]
				- v[IX(i, j - 1)]
				) / N;
			p[IX(i, j)] = 0;
		}
	}
	
	set_bnd(0, div, N);
	set_bnd(0, p, N);
	lin_solve(0, p, div, 1, 4, iter, N);

	for (int j = 1; j < N - 1; j++) {
		for (int i = 1; i < N - 1; i++) {
			u[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)]
				- p[IX(i - 1, j)]) * N;
			v[IX(i, j, k)] -= 0.5f * (p[IX(i, j + 1)]
				- p[IX(i, j - 1)]) * N;
		}
	}
	
	set_bnd(1, u, N);
	set_bnd(2, v, N);

}

void SimulationMA::vel_step(int N, float* u, float* v, float* u0, float* v0, float visc, float dt)
{
	//add_source(N, u, u0, dt); add_source(N, v, v0, dt);
	diffuse(1, u0, u, visc, dt, 4, N);
	diffuse(2, v0, v, visc, dt, 4, N);
	project(u0, v0, u, v, 4, N);
	advect(1, u, u0, u0, v0, dt, N);
	advect(2, v, v0, u0, v0, dt, N);
	project(u, v, u0, v0, 4, N);
}

void SimulationMA::dens_step(int N, float* x, float* x0, float* u, float* v, float diff, float dt)
{
	//add_source(N, x, x0, dt);
	print(x, "poczatek:");
	diffuse(0, x0, x, diff, dt, 4, N);
	print(x, "po dyfuzji:");
	//adwekcja zmienia ilość2 cieczy
	advect(0, x, x0, u, v, dt, N);
	print(x, "po advekcji:");
}

void SimulationMA::print(float* x, std::string s) {
	int lns = (size + 2) * (size + 2);
	float combined = 0;
	for (int i = 0; i < lns; i++) {
		combined += x[i];
	}
	s.append(" ").append(std::to_string(combined)).append("\n\0");
	char* c = &*(s.begin());
	OutputDebugString(c);


}

void SimulationMA::lin_solve(int b, float* x, float* x0, float a, float c, int iter, int N) {
	float cRecip = 1.0 / c;
	for (int k = 0; k < iter; k++) {
		for (int j = 1; j < N - 1; j++) {
			for (int i = 1; i < N - 1; i++) {
				x[IX(i, j)] =
					(x0[IX(i, j)]
						+ a * (x[IX(i + 1, j)]
							+ x[IX(i - 1, j)]
							+ x[IX(i, j + 1)]
							+ x[IX(i, j - 1)]
							)) * cRecip;
			}
		}
		set_bnd(b, x, N);
	}
		
}
