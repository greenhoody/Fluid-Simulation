#include "accgpu.h"
#include <cstring>

#define IX(i,j) ((i)+(N+2)*(j))


#pragma acc routine vector
void set_bnd(int N, int b, float* x)
{

    int n = (N + 2) * (N + 2);

    #pragma acc loop independent // copy(x[0:n]) /**/ //
    for (int i = 1; i <= N; i++) {
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

void diffuse(int N, int b, float* x, float* x0, float diff, float dt)
{
    int k,t;
    float a = dt * diff * N * N;
    int n = (N + 2) * (N + 2);
    #pragma acc data copyin(x0[0:n]) copy(x[0:n])
    {
        for (k = 0; k < 20; k++) 
        {
            #pragma acc parallel loop independent collapse(2)
            for (int i = 1; i <= N; i++)
            {
                for (int j = 1; j <= N; j++)
                {
                    x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
                        x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + (4 * a));
                }
            }
            set_bnd(N, b, x);
        }
    }

    
}

void advect(int N, int b, float *d, float *d0, float *u, float *v, float dt)
{
    int i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;
    dt0 = dt * (float)N;

    int n = (N + 2) * (N + 2);
    #pragma acc data copyin(d0[0:n],u[0:n],v[0:n]) copy(d[0:n])
    {
        #pragma acc parallel loop independent collapse(2) 
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

            }
        }
    set_bnd(N, b, d);
    }  
}

void project(int N, float *u, float *v, float *p, float *div)
{
    int i, j, k;
    float h;
    h = 1.0f / N;
    int n = (N + 2) * (N + 2);

    #pragma acc data copy(u[0:n],v[0:n],p[0:n],div[0:n])
    {
        #pragma acc parallel loop independent collapse(2)
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= N; j++) {
                div[IX(i, j)] = -0.5f * h * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
                    v[IX(i, j + 1)] - v[IX(i, j - 1)]);
                p[IX(i, j)] = 0.0f;
            }
        }

        set_bnd(N, 0, div);
        set_bnd(N, 0, p);

        for (k = 0; k < 20; k++)
        {
        #pragma acc parallel loop independent collapse(2)
            for (i = 1; i <= N; i++) {
                for (j = 1; j <= N; j++) {
                    p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
                        p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
                }
            }
            set_bnd(N, 0, p);
        }
        #pragma acc parallel loop independent collapse(2)
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= N; j++) {
                u[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
                v[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
            }
        }
        set_bnd(N, 1, u);
        set_bnd(N, 2, v);
    }
}

void NextFrameACC(int N, float* copy_array, float* dens, float* dens_prev, float* u, float* v, 
                    float* u0, float* v0, float diff, float visc, float dt)
{
    diffuse(N, 1, u0, u, visc, dt);
    diffuse(N, 2, v0, v, visc, dt);
    project(N, u0, v0, u, v);
    advect(N, 1, u, u0, u0, v0, dt);
    advect(N, 2, v, v0, u0, v0, dt);
    project(N, u, v, u0, v0);

    diffuse(N, 0, dens_prev, dens, diff, dt);
    //adwekcja zmienia ilość cieczy
    advect(N, 0, dens, dens_prev, u, v, dt);
    memcpy(copy_array, dens, sizeof(float) * (N + 2) * (N + 2));
}