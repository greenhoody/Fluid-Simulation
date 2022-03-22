#include "Simulation.h"
#include"pch.h"

class SimulationMA : public Simulation {
public:
	SimulationMA(int size, float diffiusion, float viscosity, float dt);
	~SimulationMA();
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
	void project(std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, std::shared_ptr<float[]> p, std::shared_ptr<float[]> div, int iter, int N);
	void advect( int b, std::shared_ptr<float[]> d, std::shared_ptr<float[]> d0, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, float dt, int N);
	void diffuse(int b, std::shared_ptr<float[]> x, std::shared_ptr<float[]> x0, float diff, float dt, int iter, int N);
	void set_bnd(int b, std::shared_ptr<float[]> x, int N);
	void add_source(int N, std::shared_ptr<float[]> x, std::shared_ptr<float[]> s, float dt);

	// do testowania
	void lin_solve(int b, std::shared_ptr<float[]> x, std::shared_ptr<float[]> x0, float a, float c, int iter, int N);

	//testowa
	void print(std::shared_ptr<float[]> x, std::string prefix);
};