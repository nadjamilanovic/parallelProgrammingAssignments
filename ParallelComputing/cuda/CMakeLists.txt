cmake_minimum_required(VERSION 3.18)
project(BabelStreamCUDA LANGUAGES CXX CUDA)

add_executable(cuda-stream main.cpp CUDAStream.cu)
set_target_properties(cuda-stream PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_compile_definitions(cuda-stream PRIVATE IMPLEMENTATION_STRING="CUDA")
