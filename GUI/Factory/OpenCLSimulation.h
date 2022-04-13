#pragma once
#include "Simulation.h"
#include "pch.h"

class OpenCLSimulation : public Simulation
{
public:
	float* u, * v, * u_prev, * v_prev, * u_const, * v_const, * dens, * dens_prev, * dens_const;

	OpenCLSimulation(int size, float diffiusion, float viscosity, float dt);
	~OpenCLSimulation();
	void NextFrame(std::shared_ptr<float[]> copy_array);
	void AddDensity(int x, int y, float density);
	void AddVelocity(int x, int y, float v_velocity, float h_velocity);
	void AddConstantDensity(int x, int y, float density);
	void DeleteConstantDensity(int x, int y);
	void AddConstantVelocity(int x, int y, float v_velocity, float h_velocity);
	void DeleteConstantVelocity(int x, int y);
private:
	void vel_step(int N, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, std::shared_ptr<float[]> u0, std::shared_ptr<float[]> v0, float visc, float dt);
	void dens_step(int N, std::shared_ptr<float[]> x, std::shared_ptr<float[]> x0, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, float diff, float dt);
	void project(int N, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, std::shared_ptr<float[]> p, std::shared_ptr<float[]> div);
	void advect(int N, int b, std::shared_ptr<float[]> d, std::shared_ptr<float[]> d0, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, float dt);
	void diffuse(int N, int b, std::shared_ptr<float[]> x, std::shared_ptr<float[]> x0, float diff, float dt);
	void set_bnd(int N, int b, std::shared_ptr<float[]> x);
	void add_source(int N, std::shared_ptr<float[]> x, std::shared_ptr<float[]> s, float dt);

};