#include "openaccsimulation.h"
#include <string.h>

#include "accgpu.h"

#define IX(i,j) ((i)+(size+2)*(j))

OpenACCSimulation::OpenACCSimulation(int size, float diffiusion, float viscosity, float dt):Simulation(size, diffiusion, viscosity, dt)
{
    this->u = (float*)calloc(n, sizeof(float));
    this->u_prev = (float*)calloc(n, sizeof(float));
    this->v = (float*)calloc(n, sizeof(float));
    this->v_prev = (float*)calloc(n, sizeof(float));
    this->dens = (float*)calloc(n, sizeof(float));
    this->dens_prev = (float*)calloc(n, sizeof(float));

}

OpenACCSimulation::~OpenACCSimulation()
{
    free(u);
    free(u_prev);
    free(v);
    free(v_prev);
    free(dens);
    free(dens_prev);
}

void OpenACCSimulation::NextFrame(float* copy_array)
{
    NextFrameACC(size, copy_array, dens, dens_prev, u,v, u_prev, v_prev, diff, visc, dt);
}

void OpenACCSimulation::AddDensity(int x1, int x2, int y1, int y2, float density)
{
    if(x1 > x2)
    {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    if (y1 > y2)
    {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    x1 = x1 < 0 ? 0 : x1;
    y1 = y1 < 0 ? 0 : y1;
    x2 = x2 > size ? size : x2;
    y2 = y2 > size ? size : y2;

    for (int i = x1; i <= x2; i++)
    {
        for (int j = y1; j <= y2; j++)
        {
            // to dodawanie jest potrzbne ponieważ, pierwszy rząd i kolumna nie sa pokazywane, więc obraz jest przesunięty
            int index = IX(i + 1, j + 1);
            dens[index] += density;
            if (dens[index] > 1) {
                dens[index] = 1;
            }
        }
    }
}

void OpenACCSimulation::AddVelocity(int x, int y, int r, float v_velocity, float h_velocity)
{
    int r_square = r * r;

    for (int i = x - r; i <= x + r; i++)
    {
        for (int j = y - r; j <= y + r; j++)
        {
            if (size > i && i > 0 && size > j && j > 0 && ((i-x) * (i-x)) + ((j-y) * (j-y)) <= r_square) {
                // to dodawanie jest potrzebne ponieważ, pierwszy rząd i kolumna nie sa pokazywane, więc obraz jest przesunięty
                int index = IX(i + 1, j + 1);
                v[index] += v_velocity;
                u[index] += h_velocity;
            }
        }
    }
}
