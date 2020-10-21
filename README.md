# RISC-V Vectorized Bencmark Suite

## Overview

The RISC-V Vectorized Benchmark Suite is a collection composed of seven data-parallel applications from different domains. The suite focuses on benchmarking vector microarchitectures; nevertheless, it can be used as well for Multimedia SIMD microarchitectures. Current implementation is targeting RISC-V Architectures; however, it can be easily ported to any Vector/SIMD ISA thanks to a wrapper library which we developed to map vector intrinsics and math functions to the target architecture.

The benchmark suite with all its applications and input sets is available as open source free of charge. Some of the benchmark programs have their own licensing terms which might limit their use in some cases.

The implementation is based on the Working draft of the proposed RISC-V V vector extension v0.7.1
https://github.com/riscv/riscv-v-spec/releases/tag/0.7.1


If you use this software or a modified version of it for your research, please cite the paper:
Cristóbal Ramirez, César Hernandez, Oscar Palomar, Osman Unsal, Marco Ramírez, and Adrián Cristal. 2020. A RISC-V Simulator and Benchmark Suite for Designing and Evaluating Vector Architectures. ACM Trans. Archit. Code Optim. 17, 4, Article 38 (October 2020), 29 pages. https://doi.org/10.1145/3422667

## Pakage Structure

    Vectorized_Benchmark_Suite/ : The Vectorized Benchmark Suite
        blackscholes/           : Blackscholes main folder
        ...../src               : Blackscholes sources
        ...../bin               : Blackscholes binary
        canneal/                : canneal main folder
        ...../src               : canneal sources
        ...../bin               : canneal binary
        heatequation/           : heatequation main folder
        ...../src               : heatequation sources
        ...../bin               : heatequation binary
        particlefilter/         : particlefilter main folder
        ...../src               : particlefilter sources
        ...../bin               : particlefilter binary
        pathfinder/             : pathfinder main folder
        ...../src               : pathfinder sources
        ...../bin               : pathfinder binary
        streamcluster/          : streamcluster main folder
        ...../src               : streamcluster sources
        ...../bin               : streamcluster binary
        swaptions/              : swaptions main folder
        ...../src               : swaptions sources
        ...../bin               : swaptions binary
        common/                 : intrinsics defines & Math functions

## Building Vectorized Applications 

The RISC-V Vectorized Bencmark Suite has been successfully tested on Ubuntu 18 and Centos7

### Setting up the environment

The Suite includes a makefile to compile every application, in order to use it, you must define the path to the RISC-V vector compiler.

Setting the Vector Compiler path
```
export LLVM=$TOP/epi-toolchain
export PATH=$PATH:$LLVM/bin
```

### Compile using  clang for RISCV Vector Version

We provide precompiled binaries found in the folder bin.

To compile any application you first enter in the subfolder and run the command make followed by the application name
```
cd _application
make application 
```
For example to compile blackscholes:
```
cd _blackscholes
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
simsmall : takes around 15 minutes to be executed on gem5
simmedium : takes around 1  hour to be executed on gem5
simlarge : takes around 8 hours to be executed on gem5
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
heatequation_args   = " input/test_small.input output.ppm"
matmul_args         = "8 8 8"
axpy_args           = "256"
```

#### simsmall 
```
blackscholes_args   = "1 input/in_4K.input output_prices.txt"
canneal_args        = "1 10000 2000 input/100000.nets 32"
streamcluster_args  = "10 20 128 4096 4096 1000 none output.txt 1"
swaptions_args      = "-ns 8 -sm 4096 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 8 -np 1024"
pathfinder_args     = "1024 128 output.txt"
heatequation_args   = " input/test_small.input output.ppm"
matmul_args         = "128 128 128"
axpy_args           = "512"
```  

#### simmedium
```
blackscholes_args   = "1 input/in_16K.input prices.txt"
canneal_args        = "1 15000 2000 input/200000.nets 64"
streamcluster_args  = "10 20 128 8192 8192 1000 none output.txt 1"
swaptions_args      = "-ns 32 -sm 8192 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 16 -np 4096"
pathfinder_args     = "2048 256 output.txt"
heatequation_args   = " input/test_medium.input output.ppm"
matmul_args         = "256 256 256"
axpy_args           = "1024"
```  

#### simlarge
```
blackscholes_args   = "1 input/in_64K.input prices.txt"
canneal_args        = "1 15000 2000 input/400000.nets 128"
streamcluster_args  = "10 20 128 8192 8192 1000 none output.txt 1"
swaptions_args      = "-ns 64 -sm 16384 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 24 -np 8192"
pathfinder_args     = "2048 1024 output.txt"
heatequation_args   = " input/test_large.input output.ppm"
matmul_args         = "1024 1024 1024"
axpy_args           = "2048"
```  

#### native
```
blackscholes_args   = "1 input/in_10M.input prices.txt"
canneal_args        = "1 15000 2000 input/2500000.nets 6000"
streamcluster_args  = "10 20 128 1000000 200000 5000 none output.txt 1"
swaptions_args      = "-ns 128 -sm 1000000 -nt 1"
particlefilter_args = "-x 256 -y 256 -z 24 -np 65536"
pathfinder_args     = "32768 4096 output.txt"
heatequation_args   = " input/test_large.input output.ppm"
matmul_args         = "2048 2048 2048"
axpy_args           = "4096"
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

## Contact
Cristóbal Ramírez Lazo: cristobal.ramirez@bsc.es
PhD. Student at UPC Barcelona   
BSC - Barcelona Supercomputing Center
