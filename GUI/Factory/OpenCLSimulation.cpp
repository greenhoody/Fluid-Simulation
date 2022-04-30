#include "Simulation.h"
#include "CL/cl.hpp"
#include <fstream>

#include "OpenCLSimulation.h"


OpenCLSimulation::OpenCLSimulation(int size, float diffiusion, float viscosity, float dt):Simulation(size, diffiusion, viscosity, dt) 
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	auto platform = platforms.front();
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

	auto device = devices.front();

	std::ifstream clFile("OpenCLCode.cl");
	std::string src(std::istreambuf_iterator<char>(clFile), (std::istreambuf_iterator<char>()));


	std::vector<std::pair<char*, ::size_t>> kernels;
	//kernels.push_back(1,std::make_pair(src.c_str(), src.length() + 1));
	cl::Program::Sources sources(1,std::make_pair(src.c_str(), src.length() + 1));

	cl::Context context(devices);

	cl::Program program(context, sources);

	auto err = program.build();

	char buf[16];
	//cl::Buffer membuf(context, );
	//cl::Kernel kernel(program, "OpenCLCode", &err);
	//kernel.setArg(0, memBuf);

	//cl::CommandQueue queue(context, device);

	//size_t max_work_item[3];
	//device.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &max_work_item);

	//queue.enqueueNDRangeKernel(kernel,max_work_item[0],);
}

OpenCLSimulation::~OpenCLSimulation() 
{
}

void OpenCLSimulation::NextFrame(std::shared_ptr<float[]> copy_array)
{}
void OpenCLSimulation::AddDensity(int x, int y, float density)
{}
void OpenCLSimulation::AddVelocity(int x, int y, float v_velocity, float h_velocity)
{}











 //===================================================================================================
void OpenCLSimulation::AddConstantDensity(int x, int y, float density){}
void OpenCLSimulation::DeleteConstantDensity(int x, int y){}
void OpenCLSimulation::AddConstantVelocity(int x, int y, float v_velocity, float h_velocity){}
void OpenCLSimulation::DeleteConstantVelocity(int x, int y){}