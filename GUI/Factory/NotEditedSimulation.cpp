#include "NotEditedSimulation.h"
#include <corecrt_malloc.h>
#include <string.h>
#include <algorithm>
#include"pch.h"
#include <string>

#include <windows.h>
#include <debugapi.h>;


#define IX(i,j) ((i)+(size+2)*(j))

NotEditedSimulation::NotEditedSimulation(int size, float diffiusion, float viscosity, float dt): Simulation(size, diffiusion, viscosity, dt) {

}

NotEditedSimulation::~NotEditedSimulation() {
}

void NotEditedSimulation::NextFrame(std::shared_ptr<float[]> copy_array) {
	vel_step(size, u, v, u_prev, v_prev, visc, dt);
	dens_step(size, dens, dens_prev, u, v, diff, dt);
	memcpy(copy_array.get(), dens.get(), sizeof(float) * (size + 2) * (size + 2));
}

void NotEditedSimulation::AddDensity(int x, int y, float density) {
	int index = IX(x + 1, y + 1);
	dens[index] += density;
	if (dens[index] > 1) {
		dens[index] = 1;
	}
}

void NotEditedSimulation::AddVelocity(int x, int y, float h_velocity, float v_velocity) {
	int index = IX(x + 1, y + 1);
	//v z indeksem ujemnym wpisuje dane do u_prev/ mazanie po pamięci
	v[index] += v_velocity;
	u[index] += h_velocity;
}

//=====================================================================================================================

void NotEditedSimulation::add_source(int N, std::shared_ptr<float[]> x, std::shared_ptr<float[]> s, float dt)
{
	int i, size = (N + 2) * (N + 2);
	for (i = 0; i < size; i++) x[i] += dt * s[i];
}

void NotEditedSimulation::set_bnd(int N, int b, std::shared_ptr<float[]> x)
{
	int i;
	for (i = 1; i <= N; i++) {
		x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
		x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
		x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
		x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
	}
	x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
	x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
	x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
	x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

void NotEditedSimulation::diffuse(int N, int b, std::shared_ptr<float[]> x, std::shared_ptr<float[]> x0, float diff, float dt)
{
	int i, j, k;
	float a = dt * diff * N * N;
	for (k = 0; k < 20; k++) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
					x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + (4 * a));
			}
		}
		set_bnd(N, b, x);
	}
}

void NotEditedSimulation::advect(int N, int b, std::shared_ptr<float[]> d, std::shared_ptr<float[]> d0, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, float dt)
{
	int i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;
	dt0 = dt * (float)N;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {

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


			//TEST
			if (i == 100 && j == 100) {
				printf("dupa");
			}

		}
	}
	set_bnd(N, b, d);
}

void NotEditedSimulation::project(int N, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, std::shared_ptr<float[]> p, std::shared_ptr<float[]> div)
{
	int i, j, k;
	float h;
	h = 1.0f / N;
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			div[IX(i, j)] = -0.5f * h * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
				v[IX(i, j + 1)] - v[IX(i, j - 1)]);
			p[IX(i, j)] = 0.0f;
		}
	}
	set_bnd(N, 0, div); 
	set_bnd(N, 0, p);


	for (k = 0; k < 20; k++) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
					p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
			}
		}
		set_bnd(N, 0, p);
	}
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			u[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
			v[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
		}
	}
	set_bnd(N, 1, u); 
	set_bnd(N, 2, v);
}

void NotEditedSimulation::vel_step(int N, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, std::shared_ptr<float[]> u0, std::shared_ptr<float[]> v0, float visc, float dt)
{
	//add_source(N, u, u0, dt); add_source(N, v, v0, dt);
	diffuse(N, 1, u0, u, visc, dt);
	diffuse(N, 2, v0, v, visc, dt);
	project(N, u0, v0, u, v);
	advect(N, 1, u, u0, u0, v0, dt);
	advect(N, 2, v, v0, u0, v0, dt);
	project(N, u, v, u0, v0);
}

void NotEditedSimulation::dens_step(int N, std::shared_ptr<float[]> x, std::shared_ptr<float[]> x0, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, float diff, float dt)
{
	//add_source(N, x, x0, dt);
	print(x, "poczatek:");
	diffuse(N, 0, x0, x, diff, dt);
	print(x, "po dyfuzji:");
	//adwekcja zmienia ilość2 cieczy
	advect(N, 0, x, x0, u, v, dt);
	print(x, "po advekcji:");
}

void NotEditedSimulation::print(std::shared_ptr<float[]> x, std::string s) {
	int lns = (size + 2) * (size + 2);
	float combined = 0;
	for (int i = 0; i < lns; i++) {
		combined += x[i];
	}
	s.append(" ").append(std::to_string(combined)).append("\n\0");
	char* c = &*(s.begin());
	OutputDebugString(c);


}

void NotEditedSimulation::AddConstantDensity(int x, int y, float density) {}
void NotEditedSimulation::DeleteConstantDensity(int x, int y) {}
void NotEditedSimulation::AddConstantVelocity(int x, int y, float v_velocity, float h_velocity) {}
void NotEditedSimulation::DeleteConstantVelocity(int x, int y) {}