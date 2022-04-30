#ifndef ACCGPU_H
#define ACCGPU_H

void set_bnd(int N, int b, float *x);
void diffuse(int N, int b, float* x, float* x0, float diff, float dt);
void advect(int N, int b, float *d, float *d0, float *u, float *v, float dt);
void project(int N, float *u, float *v, float *p, float *div);
void NextFrameACC(int N, float* copy_array, float* dens, float* dens_prev, float* u, float* v, float* u0, float* v0, float diff, float visc, float dt);

#endif // ACCGPU_H
