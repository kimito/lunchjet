#include "drive_detector.h"

#include <fstream>
#include <memory>
#include <numeric>

#include <cuda_runtime_api.h>

#include "debug_log.h"
#include "performance_count.h"

namespace lunchjet {

static size_t getMemorySize(const nvinfer1::Dims& dims, const int32_t elem_size)
{
    return std::accumulate(dims.d, dims.d + dims.nbDims, 1, std::multiplies<int64_t>()) * elem_size;
}

class Logger : public nvinfer1::ILogger           
 {
     void log(Severity severity, const char* msg) override
     {
         if (severity < Severity::kINFO)
            debug_debug("TensorRT log: %d %s", static_cast<int>(severity) ,msg);
     }
};
static Logger logger;
static DriveParameter value_on_error = {0.f, 0.f};


DriveDetector::DriveDetector(const std::string &model_file_path)
:engine_file_path(model_file_path),
 engine(nullptr),
 context(nullptr),
 input_mem_size(0),
 output_mem_size(0),
 input_mem(nullptr),
 output_mem(nullptr),
 stream()
{
    std::ifstream engine_file(engine_file_path, std::ios::binary);
    if (engine_file.fail())
    {
        return;
    }

    //getting size of engine file
    engine_file.seekg(0, std::ifstream::end);
    auto fsize = engine_file.tellg();
    engine_file.seekg(0, std::ifstream::beg);

    std::vector<char> engine_data(fsize);
    engine_file.read(engine_data.data(), fsize);

    //get engine
    unique_ptr_for_nv<nvinfer1::IRuntime> runtime{nvinfer1::createInferRuntime(logger)};
    engine.reset(runtime->deserializeCudaEngine(engine_data.data(), fsize, nullptr));
    if(engine == nullptr) {
        std::cout << "deserializeCudaEngine() failed" << std::endl;
    }

    //get context
    context.reset(engine->createExecutionContext());
    if(!context) {
        debug_err("createExecutionContext() failed");
    }

    //configure input tensor
    std::string input_name("input:0");
    auto input_index = engine->getBindingIndex(input_name.c_str());
    std::cout << "input index: " << input_index << std::endl;
    if(input_index == -1) {
        debug_err("getBindingIndex() for input failed: %s", input_name.c_str());
    }
    auto input_dims = context->getBindingDimensions(input_index);
    input_mem_size = getMemorySize(input_dims, sizeof(float));

    //configure output tensor
    std::string output_name("output");
    auto output_index = engine->getBindingIndex(output_name.c_str());
    std::cout << "output index: " << output_index << std::endl;
    if(output_index == -1) {
        debug_err("getBindingIndex() for output failed: %s", output_name.c_str());
    }
    auto output_dims = context->getBindingDimensions(output_index);
    output_mem_size = getMemorySize(output_dims, sizeof(float));

    // Allocate CUDA memory for input and output bindings
    if (cudaMalloc(&input_mem, input_mem_size) != cudaSuccess)
    {
        debug_err("input cuda memory allocation failed, size = %zu bytes", input_mem_size);
    }
    if (cudaMalloc(&output_mem, output_mem_size) != cudaSuccess)
    {
        debug_err("ERROR: output cuda memory allocation failed, size = %zu bytes", output_mem_size);
    }

    cudaStream_t stream;
    if (cudaStreamCreate(&stream) != cudaSuccess)
    {
        debug_debug("ERROR: cuda stream creation failed.");
    }
}


DriveDetector::~DriveDetector()
{
    cudaFree(input_mem);
    cudaFree(output_mem);
}


DriveParameter DriveDetector::detect(const cv::Mat &image)
{
    cv::Mat image_rgb, image_rgb_float;
    cv::cvtColor(image, image_rgb, cv::COLOR_BGR2RGB);
    image_rgb.convertTo(image_rgb_float, CV_32F);

    // Copy image data to input binding memory
    if (cudaMemcpyAsync(input_mem, image_rgb_float.data, input_mem_size, cudaMemcpyHostToDevice, stream) != cudaSuccess)
    {
        debug_debug("ERROR: CUDA memory copy of input failed, size = %zu bytes", input_mem_size);
        return value_on_error;
    }

    // Run TensorRT inference
    void* bindings[] = {input_mem, output_mem};
    if (context->enqueueV2(bindings, stream, nullptr) == false)
    {
        debug_debug("ERROR: TensorRT inference failed");
        return value_on_error;
    }

    // Copy predictions from output binding memory
    auto output_buffer = std::unique_ptr<float[]>{new float[output_mem_size]};
    if (cudaMemcpyAsync(output_buffer.get(), output_mem, output_mem_size, cudaMemcpyDeviceToHost, stream) != cudaSuccess)
    {
        debug_debug("ERROR: CUDA memory copy of output failed, size = %zu bytes", output_mem_size);
        return value_on_error;
    }
    cudaStreamSynchronize(stream);

    return {output_buffer[0], output_buffer[1]};
}

} //namespace lunchjet