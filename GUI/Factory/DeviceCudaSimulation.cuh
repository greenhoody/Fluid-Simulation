
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void cuda_NextFrame(int N, float* d_dens, float* d_dens_prev, float* d_u, float* d_v, float* d_u_prev, float* d_v_prev, float visc, float diff, float dt);
__global__ void addDensity(int N, float* d_dens, int x1, int x2, int y1, int y2, float dens);
__global__ void addVelocity(int N, float* d_u, float* d_v,int x, int y, int r, float u_velocity, float v_velocity);

__device__ void project(int N, float* u, float* v, float* p, float* div);
__device__ void advect(int N, int b, float* d, float* d0, float* u, float* v, float dt);
__device__ void diffuse(int N, int b, float* x, float* x0, float diff, float dt);
__device__ void set_bnd(int N, int b, float* x);


//__device__ void vel_step(int N, float* u, float* v, float* u0, float* v0, float visc, float dt);
//__device__ void dens_step(int N, float* x, float* x0, float* u, float* v, float diff, float dt);