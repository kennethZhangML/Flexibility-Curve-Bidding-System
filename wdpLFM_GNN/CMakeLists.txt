cmake_minimum_required(VERSION 3.20)
project(wdpLFM_GNN)

set(CMAKE_CXX_STANDARD 17)

# Set the path to your libtorch installation
set(LibTorch_DIR /Users/kennethzhang/Desktop/libtorch)

find_package(Torch REQUIRED PATHS ${LibTorch_DIR})

add_executable(wdpLFM_GNN main.cpp
        flexibilityCurveTree.cpp
        flexibilityCurveTree.h
        testGNN.cpp
        testGNN.h)

target_link_libraries(wdpLFM_GNN ${TORCH_LIBRARIES})
