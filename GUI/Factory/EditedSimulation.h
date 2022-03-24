#pragma once
#include "Simulation.h"

class EditedSimulation : public Simulation {
public :
	EditedSimulation(int size, float diffiusion, float viscosity, float dt);
	~EditedSimulation();
	void NextFrame(std::shared_ptr<float[]> copy_array);
	void AddDensity(int x, int y, float density);
	void AddVelocity(int x, int y, float v_velocity, float h_velocity);
	void AddWall(int x, int y);
	void DeleteWall(int x, int y);
	void AddConstantDensity(int x, int y, float density);
	void DeleteConstantDensity(int x, int y);
	void AddConstantVelocity(int x, int y, float v_velocity, float h_velocity);
	void DeleteConstantVelocity(int x, int y);
private:
	bool* walls;

	void vel_step(std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, std::shared_ptr<float[]> u0, std::shared_ptr<float[]> v0, float visc);
	void dens_step(std::shared_ptr<float[]> x, std::shared_ptr<float[]> x0, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, float diff);
	void project(std::shared_ptr<float[]> u, std::shared_ptr<float[]> v, std::shared_ptr<float[]> p, std::shared_ptr<float[]> div);
	void advect(int b, std::shared_ptr<float[]> d, std::shared_ptr<float[]> d0, std::shared_ptr<float[]> u, std::shared_ptr<float[]> v);
	void diffuse(int b, std::shared_ptr<float[]> x, std::shared_ptr<float[]> x0, float diff);
	void set_bnd(int b, std::shared_ptr<float[]> x);
	void constant (std::shared_ptr<float[]> x, std::shared_ptr<float[]> c);
	void constant_no_limit(std::shared_ptr<float[]> x, std::shared_ptr<float[]> c);
};