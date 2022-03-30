#include <cuda_runtime.h>
#include "DeviceCudaSimulation.cuh"
#include <cooperative_groups.h>
#include <cuda_runtime_api.h> 
#include <cuda.h>


#define IX(i,j) ((i)+(N+2)*(j))

//CudaSimulation::CudaSimulation(int size, float diffiusion, float viscosity, float dt) : Simulation(size, diffiusion, viscosity, dt) {
//	n = (size + 2) * (size + 2);
//	
//	block_no = (n / 1024);
//
//	// w cuda nie ma odpowiednika calloc
//	
//	cudaMalloc(&d_u, sizeof(float) * n);
//	cudaMalloc(&d_v, sizeof(float) * n);
//	cudaMalloc(&d_u_prev, sizeof(float) * n);
//	cudaMalloc(&d_v_prev, sizeof(float) * n);
//	cudaMalloc(&d_dens, sizeof(float) * n);
//	cudaMalloc(&d_dens_prev, sizeof(float) * n);
//
//	cudaMalloc(&d_u_const, sizeof(float) * n);
//	cudaMalloc(&d_v_const, sizeof(float) * n);
//	cudaMalloc(&d_dens_const, sizeof(float) * n);
//
//	cudaMemset(d_u, 0, sizeof(float) * n);
//	cudaMemset(d_v, 0, sizeof(float) * n);
//	cudaMemset(d_u_prev, 0, sizeof(float) * n);
//	cudaMemset(d_v_prev, 0, sizeof(float) * n);
//	cudaMemset(d_dens, 0, sizeof(float) * n);
//	cudaMemset(d_dens_prev, 0, sizeof(float) * n);
//
//	cudaMemset(d_u_const, 0, sizeof(float) * n);
//	cudaMemset(d_v_const, 0, sizeof(float) * n);
//	cudaMemset(d_dens_const, 0, sizeof(float) * n);
//	
//}
//
//CudaSimulation::~CudaSimulation() {
//	cudaFree(d_u);
//	cudaFree(d_v);
//	cudaFree(d_u_prev);
//	cudaFree(d_v_prev);
//	cudaFree(d_dens);
//	cudaFree(d_dens_prev);
//
//	cudaFree(d_u_const);
//	cudaFree(d_v_const);
//	cudaFree(d_dens_const);
//}

//void CudaSimulation::NextFrame(std::shared_ptr<float[]> copy_array) {
//	
//	int blocks = ceilf((float)n / 1024);
//	dim3 gridDim = dim3(blocks,1,1);
//	dim3 blockDim = dim3(1024, 1, 1);
//	void* kernelArgs = {};
//
//	cudaLaunchCooperativeKernel(  (void*) CudaSimulation::cuda_NextFrame, gridDim, blockDim, kernelArgs);
//
//	cuda_NextFrame<<<blocks, 1024>>>();
//	cudaMemcpy(copy_array.get(), d_dens, sizeof(float) * (size + 2) * (size + 2), cudaMemcpyDeviceToHost);
//}


//=====================================================================================================================

//void CudaSimulation::add_source(int N, float* x, float* s, float dt)
//{
//	int i, size = (N + 2) * (N + 2);
//	for (i = 0; i < size; i++) x[i] += dt * s[i];
//}


// wątek zero rogi, reszta krawędzie, wiem że nie optymalne, ale może późjniej nad tym pomyśle i wywali się przy zamałej ilości wątków, na początku synchronizacja grid. Ta funkcja jest wywołyuwana wiele razy, więc wiele synchronizacji więc trzeba sprawdzić póxniej czy potrzebne
__device__ void set_bnd(int N, int b, float* x)
{
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	
	cooperative_groups::grid_group g = cooperative_groups::this_grid();
	g.sync();

	if (index > 0 && index < N + 2) {
		x[IX(0, index)] = b == 1 ? -x[IX(1, index)] : x[IX(1, index)];
		x[IX(N + 1, index)] = b == 1 ? -x[IX(N, index)] : x[IX(N, index)];
		x[IX(index, 0)] = b == 2 ? -x[IX(index, 1)] : x[IX(index, 1)];
		x[IX(index, N + 1)] = b == 2 ? -x[IX(index, N)] : x[IX(index, N)];
	}
	
	//for (int i = 1; i <= N; i++) {
	//	x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
	//	x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
	//	x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
	//	x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
	//}

	if (index == 0)
	{
		x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
		x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
		x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
		x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
	}

}

__device__ void diffuse(int N, int b, float* x, float* x0, float diff, float dt)
{
	float a = dt * diff * N * N;
	
	int index = blockIdx.x * blockDim.x + threadIdx.x;

	// +1 po od 1 zaczynały się pętle, a nie od zera
	int j = (index / N) + 1;
	int i = (index % N) + 1;


	//kompiluje się czyli intellisense nie ogarnia
	cooperative_groups::grid_group g = cooperative_groups::this_grid();

	for (int k = 0; k < 20; k++) {
		g.sync();

		if (j > N) {
			return;
		}

		//for (int i = 1; i <= N; i++) {
		//	for (int j = 1; j <= N; j++) {
		x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
			x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + (4 * a));
		//	}
		//}
		set_bnd(N, b, x);
	}
}

__device__ void advect(int N, int b, float* d, float* d0, float* u, float* v, float dt)
{
	cooperative_groups::grid_group g = cooperative_groups::this_grid();

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	// +1 po od 1 zaczynały się pętle, a nie od zera
	int j = (index / N) + 1;
	int i = (index % N) + 1;

	if (j > N) {
		return;
	}

	int i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;
	dt0 = dt * (float)N;
	//for (int i = 1; i <= N; i++) {
	//	for (int j = 1; j <= N; j++) {

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

	//	}
	//}
	set_bnd(N, b, d);
}

__device__ void project(int N, float* u, float* v, float* p, float* div)
{

	cooperative_groups::grid_group g = cooperative_groups::this_grid();
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	// +1 po od 1 zaczynały się pętle, a nie od zera
	int j = (index / N) + 1;
	int i = (index % N) + 1;

	if (j > N) {
		return;
	}


	int k;
	float h;
	h = 1.0f / N;
	//for (i = 1; i <= N; i++) {
	//	for (j = 1; j <= N; j++) {
			div[IX(i, j)] = -0.5f * h * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
				v[IX(i, j + 1)] - v[IX(i, j - 1)]);
			p[IX(i, j)] = 0.0f;
	//	}
	//}
	set_bnd(N, 0, div);
	set_bnd(N, 0, p);


	for (k = 0; k < 20; k++) {
		g.sync();
		//for (i = 1; i <= N; i++) {
		//	for (j = 1; j <= N; j++) {
				p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
					p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
		//	}
		//}
		set_bnd(N, 0, p);
	}
	//for (i = 1; i <= N; i++) {
	//	for (j = 1; j <= N; j++) {
			u[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
			v[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
	//	}
	//}
	set_bnd(N, 1, u);
	set_bnd(N, 2, v);
}

__global__ void cuda_NextFrame(int N, float* d_dens, float* d_dens_prev, float* d_u, float* d_v, float* d_u_prev, float* d_v_prev, float visc, float diff , float dt) {
	//vel_step(size, d_u, d_v, d_u_prev, d_v_prev, visc, dt);
	
	diffuse(N, 1, d_u_prev, d_u, visc, dt);
	diffuse(N, 2, d_v_prev, d_v, visc, dt);
	project(N, d_u_prev, d_v_prev, d_u, d_v);
	advect(N, 1, d_u, d_u_prev, d_u_prev, d_v_prev, dt);
	advect(N, 2, d_v, d_v_prev, d_u_prev, d_v_prev, dt);
	project(N, d_u, d_v, d_u_prev, d_v_prev);
	
	
	//dens_step(size, d_dens, d_dens_prev, d_u, d_v, diff, dt);

	diffuse(N, 0, d_dens_prev, d_dens, diff, dt);
	advect(N, 0, d_dens, d_dens_prev, d_u, d_v, dt);
}

__global__ void addDensity(int N, float* d_dens, int x1, int x2, int y1, int y2, float dens)
{
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int deltay = y2 - y1;
	int deltax = x2 - x1;
	int j = (index / deltax);
	int i = (index % deltax);

	if (j > deltay)
	{
		return;
	}

	d_dens[IX(x1 + i,y1 + j)] += dens;
	
	if (d_dens[IX(x1 + i, y1 + j)] > 1)
	{
		d_dens[IX(x1 + i, y1 + j)] = 1;
	}
}

__global__ void addVelocity(int N, float* d_u, float* d_v, int x, int y, int r, float u_velocity, float v_velocity)
{
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int j = (index / (r*2));
	int i = (index % (r*2));
	int r_square = r * r;

	if (N > x + i && x + i > 0 && N > y + j && y + j > 0 && i * i + j * j < r_square)
	{
		d_u[IX(x + i, y + j)] += u_velocity;
		d_v[IX(x + i, y + j)] += v_velocity;
	}
}
//__device__ void vel_step(int N, float* u, float* v, float* u0, float* v0, float visc, float dt)
//{
//	//add_source(N, u, u0, dt); add_source(N, v, v0, dt);
//	diffuse(N, 1, u0, u, visc, dt);
//	diffuse(N, 2, v0, v, visc, dt);
//	project(N, u0, v0, u, v);
//	advect(N, 1, u, u0, u0, v0, dt);
//	advect(N, 2, v, v0, u0, v0, dt);
//	project(N, u, v, u0, v0);
//}

//__device__ void dens_step(int N, float* x, float* x0, float* u, float* v, float diff, float dt)
//{
//	//add_source(N, x, x0, dt);
//	//print(x, "poczatek:");
//	diffuse(N, 0, x0, x, diff, dt);
//	//print(x, "po dyfuzji:");
//	//adwekcja zmienia ilość2 cieczy
//	advect(N, 0, x, x0, u, v, dt);
//	//print(x, "po advekcji:");
//}