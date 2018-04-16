cmake_minimum_required(VERSION 3.9)
project(mpi_lab_01)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_C_COMPILER mpicc)

add_executable(mpi_lab_01
        hello-world-seq.c)
