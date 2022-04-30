#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <memory>

class Simulation
{
public:
    int size, n;
    float diff, visc, dt;
    virtual void NextFrame(float* copy_array) = 0;
    virtual void AddDensity(int x1, int x2, int y1, int y2, float density) = 0;
    virtual void AddVelocity(int x, int y, int r, float v_velocity, float h_velocity) = 0;
    virtual ~Simulation() {};

    Simulation(int size, float diffiusion, float viscosity, float dt) {
        this->size = size;
        this->diff = diffiusion;
        this->visc = viscosity;
        this->dt = dt;
        n = (size + 2) * (size + 2);
    };
};

#endif // SIMULATION_H
