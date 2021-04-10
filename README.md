# RiVEC Bencmark Suite

## Overview

The RiVEC Benchmark Suite is a collection composed of data-parallel applications from different domains. The suite focuses on benchmarking vector microarchitectures; nevertheless, it can be used as well for Multimedia SIMD microarchitectures. Current implementation is targeting RISC-V Architectures; however, it can be easily ported to any Vector/SIMD ISA thanks to a wrapper library which we developed to map vector intrinsics and math functions to the target architecture.

The benchmark suite with all its applications and input sets is available as open source free of charge. Some of the benchmark programs have their own licensing terms which might limit their use in some cases.

Currently there are two implementations based on the Working draft of the proposed RISC-V V vector extension [v0.7](https://github.com/riscv/riscv-v-spec/releases/tag/0.7.1) and [v1.0](https://github.com/riscv/riscv-v-spec):

Vectorized Bencmark Suite for rvv-0.7. Click [here](https://github.com/RALC88/riscv-vectorized-benchmark-suite/tree/rvv-0.7).

Vectorized Bencmark Suite for rvv-1.0. Click [here](https://github.com/RALC88/riscv-vectorized-benchmark-suite/tree/rvv-1.0).

If you use this software or a modified version of it for your research, please cite the paper:
Cristóbal Ramírez, César Hernandez, Oscar Palomar, Osman Unsal, Marco Ramírez, and Adrián Cristal. 2020. A RISC-V Simulator and Benchmark Suite for Designing and Evaluating Vector Architectures. ACM Trans. Archit. Code Optim. 17, 4, Article 38 (October 2020), 29 pages. https://doi.org/10.1145/3422667

## Vectorized apps

| Application Name  | Application Domain            | Algorithmic Model     | Taken From  |
| ----------------- |------------------------------ | --------------------- | ----------- |
| Axpy              | High Performance Computing    | BLAS                  | -           |
| Blackscholes      | Financial Analysis            | Dense Linear Algebra  | PARSEC      |
| Canneal           | Engineering                   | Unstructured Grids    | PARSEC      |
| LavaMD2           | Molecular Dynamics            | N-Body                | Rodinia     |
| Particle Filter   | Medical Imaging               | Structured Grids      | Rodinia     |
| Somier            | Physics Simulation            | Dense Linear Algebra  | -           |
| Jacobi-2D         | Engineering                   | Dense Linear Algebra  | PolyBench   |
| Pathfinder        | Grid Traversal                | Dynamic Programming   | Rodinia     |
| Streamcluster     | Data Mining                   | Dense Linear Algebra  | PARSEC      |
| Swaptions         | Financial Analysis            | MapReduce Regular     | PARSEC      |


## Building Vectorized Applications 


### LLVM Compiler 

We provide precompiled binaries found in the folder bin. However, it is possible to recompile the codes for both rvv versions (0.7 and 1.0).
To compile for [rvv-0.7](https://github.com/RALC88/riscv-vectorized-benchmark-suite/tree/rvv-0.7) it is possible to use the [LLVM EPI Compiler](https://ssh.hca.bsc.es/epi/ftp/?C=M;O=D). The provided binaries were compiled with the version llvm-EPI-0.7-development-toolchain-cross-2021-02-23-1523. Note that the compiler is actively in development, so after a certain time, the old versions will disappear. We recommend to use the latest version.

To compile for [rvv-1.0](https://github.com/RALC88/riscv-vectorized-benchmark-suite/tree/rvv-1.0) it is possible to use the [LLVM from PLCT](https://github.com/isrc-cas/rvv-llvm)

### EPI builtins reference

Here you can find the [EPI Builtins reference for rvv-0.7](https://repo.hca.bsc.es/gitlab/rferrer/epi-builtins-ref/-/tree/EPI-0.7) used to target RISC-V V-extension.

### Setting up the environment

The Suite includes a makefile to compile every application, in order to use it, you must define the path to the RISC-V vector compiler.

Setting the Vector Compiler path
```
export LLVM=$TOP/epi-toolchain
export PATH=$PATH:$LLVM/bin
```

### Compile using  clang for RISCV Vector Version

To compile any application run the command make followed by the application name
```
make application 
```
For example to compile blackscholes:
```
make blackscholes 
```
The same for the other applications ...

Also you can compile all the applications by typing:
```
make all 
```

### Running applications

There are provided 5 different simulation sizes (arguments to run the application). Assuming that the application is running on a simulator, is recommended to not use "native" simulation size.
```
simtiny : takes seconds to be executed on gem5
simsmall : takes around 15-30 minutes to be executed on gem5
simmedium : takes around 1-2  hour to be executed on gem5
simlarge : takes around 8-24 hours to be executed on gem5
native : only for native hardware
```

Whe you are executing an application, you must write the following arguments to run a predefined simsize.
#### simtiny 
```
blackscholes_args   = "1 input/in_512.input output_prices.txt"
canneal_args        = "1 100 300 input/100.nets 8"
streamcluster_args  = "3 10 128 128 128 10 none output.txt 1"
swaptions_args      = "-ns 8 -sm 512 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 2 -np 256"
pathfinder_args     = "32 32 output.txt"
axpy_args           = "256"
lavaMD_args         = "-cores 1 -boxes1d 1 -outputFile result.txt"
somier_args         = "2 64"
jacobi_2d_args      = "256 1 output.txt"
```

#### simsmall 
```
blackscholes_args   = "1 input/in_4K.input output_prices.txt"
canneal_args        = "1 10000 2000 input/100000.nets 32"
streamcluster_args  = "10 20 128 4096 4096 1000 none output.txt 1"
swaptions_args      = "-ns 8 -sm 4096 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 8 -np 1024"
pathfinder_args     = "1024 128 output.txt"
axpy_args           = "512"
lavaMD_args         = "-cores 1 -boxes1d 4 -outputFile result.txt"
somier_args         = "4 64"
jacobi_2d_args      = "128 10 output.txt"
```  

#### simmedium
```
blackscholes_args   = "1 input/in_16K.input prices.txt"
canneal_args        = "1 15000 2000 input/200000.nets 64"
streamcluster_args  = "10 20 128 8192 8192 1000 none output.txt 1"
swaptions_args      = "-ns 32 -sm 8192 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 16 -np 4096"
pathfinder_args     = "2048 256 output.txt"
axpy_args           = "1024"
lavaMD_args         = "-cores 1 -boxes1d 6 -outputFile result.txt"
somier_args         = "2 128"
jacobi_2d_args      = "256 100 output.txt"
```  

#### simlarge
```
blackscholes_args   = "1 input/in_64K.input prices.txt"
canneal_args        = "1 15000 2000 input/400000.nets 128"
streamcluster_args  = "10 20 128 8192 8192 1000 none output.txt 1"
swaptions_args      = "-ns 64 -sm 16384 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 24 -np 8192"
pathfinder_args     = "2048 1024 output.txt"
axpy_args           = "2048"
lavaMD_args         = "-cores 1 -boxes1d 10 -outputFile result.txt"
somier_args         = "4 128"
jacobi_2d_args      = "256 2000 output.txt"
```  

#### Example of execution blackscholes serial version.
```
./blackscholes_serial 1 input/in_64K.input prices.txt

```
#### Example of execution blackscholes vector version.
```
./blackscholes_vector 1 input/in_64K.input prices.txt

```

More info about how to execute the serial and vectorized versions on the gem5 Vector Architecture model can be found in he README file located in https://github.com/RALC88/gem5/blob/develop/src/cpu/vector_engine/README.md

## Acknowledgements
We thank the [European Processor Initiative](https://www.european-processor-initiative.eu/) (EPI) project for allowing us to use and share some versions of the vector compiler they have worked on. And the Barcelona Supercomputing Center compiler team, specially to Roger Ferrer who have always support us and solved our doubts about compiler-related issues.

## Contact
Cristóbal Ramírez Lazo: cristobal.ramirez@bsc.es
PhD. Student at UPC Barcelona   
BSC - Barcelona Supercomputing Center
