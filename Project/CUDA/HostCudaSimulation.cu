#include "HostCudaSimulation.h"
#include "DeviceCudaSimulation.cuh"

#include <cuda_runtime_api.h>

HostCudaSimulation::HostCudaSimulation(int size, float diffiusion, float viscosity, float dt):Simulation(size,diffiusion,viscosity,dt)
{	
	// w cuda nie ma odpowiednika calloc
	cudaMalloc(&u, sizeof(float) * n);
	cudaMalloc(&v, sizeof(float) * n);
	cudaMalloc(&u_prev, sizeof(float) * n);
	cudaMalloc(&v_prev, sizeof(float) * n);
	cudaMalloc(&dens, sizeof(float) * n);
	cudaMalloc(&dens_prev, sizeof(float) * n);

	cudaMemset(u, 0, sizeof(float) * n);
	cudaMemset(v, 0, sizeof(float) * n);
	cudaMemset(u_prev, 0, sizeof(float) * n);
	cudaMemset(v_prev, 0, sizeof(float) * n);
	cudaMemset(dens, 0, sizeof(float) * n);
	cudaMemset(dens_prev, 0, sizeof(float) * n);
}

HostCudaSimulation::~HostCudaSimulation()
{
	cudaFree(u);
	cudaFree(v);
	cudaFree(u_prev);
	cudaFree(v_prev);
	cudaFree(dens);
	cudaFree(dens_prev);
}

void HostCudaSimulation::NextFrame(float* copy_array) 
{


	void* kernelArgs[] = {&size, &dens, &dens_prev, &u, &v, &u_prev, &v_prev, &visc, &diff, &dt };

	int numBlocksPerSm = 0;
	// Number of threads my_kernel will be launched with
	int numThreads = 32;
	cudaDeviceProp deviceProp;
	// device zero ponieważ to zzazwyczaj najwydajniesza karta, w moim przypadku jest tylko jedna karta
	cudaGetDeviceProperties(&deviceProp, 0);
	cudaError_t e = cudaOccupancyMaxActiveBlocksPerMultiprocessor(&numBlocksPerSm, cuda_NextFrame, numThreads, 0);
	dim3 blockDim(numThreads, 1, 1);
	dim3 gridDim(deviceProp.multiProcessorCount * numBlocksPerSm, 1, 1);

	cudaLaunchCooperativeKernel((void*)cuda_NextFrame, gridDim, blockDim, kernelArgs);

	cudaDeviceSynchronize();
	cudaMemcpy(copy_array, dens, sizeof(float) * n, cudaMemcpyDeviceToHost);
}

void HostCudaSimulation::AddDensity(int x1, int x2, int y1, int y2, float density)
{
	if (x1 > x2)
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

	int delta_x = x2 - x1;
	int delta_y = y2 - y1;
	int cells = delta_x * delta_y;

	int blocks = ceilf((float)cells / 1024);
	dim3 gridDim = dim3(blocks, 1, 1);
	dim3 blockDim = dim3(1024, 1, 1);


	void* kernelArgs[] = { &size, &dens, &x1, &x2, &y1, &y2, &density };

	cudaLaunchCooperativeKernel((void*)addDensity, gridDim, blockDim, kernelArgs);
	cudaDeviceSynchronize();
}

void HostCudaSimulation::AddVelocity(int x, int y, int r, float v_velocity, float h_velocity)
{
	int cells = 4 * r * r;
	int blocks = ceilf((float)cells / 1024);
	dim3 blockDim;
	dim3 gridDim = dim3(blocks, 1, 1);
	if (blocks == 1) {
		blockDim = dim3(cells, 1, 1);
	}
	else
	{
		blockDim = dim3(1024, 1, 1);
	}

	void* kernelArgs[] = { &size,&u,&v, &x, &y, &r, &h_velocity, &v_velocity };

	cudaLaunchCooperativeKernel((void*)addVelocity, gridDim, blockDim, kernelArgs);
	cudaDeviceSynchronize();
}