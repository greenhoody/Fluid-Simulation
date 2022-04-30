libcuda.so:	
	nvcc -x --compiler-options -Wall -gencode arch=compute_70,code=sm_70 -rdc=true --shared -Xcompiler=-fPIC -o libcuda.so DeviceCudaSimulation.cu HostCudaSimulation.cu
