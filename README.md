# RiVEC Bencmark Suite

## Overview

The RiVEC Benchmark Suite is a collection composed of data-parallel applications from different domains. The suite focuses on benchmarking vector microarchitectures. Current implementation is targeting RISC-V Architectures; however, it can be easily ported to any Vector/SIMD ISA thanks to a wrapper library which we developed to map vector intrinsics and math functions to the target architecture.

The benchmark suite with all its applications and input sets is available as open source free of charge. Some of the benchmark programs have their own licensing terms which might limit their use in some cases.

RiVEC implements the lastest riscv intrinsics for rvv-1.0. It can be compiled with the latest [riscv-collab/riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain). RiVEC has been successfully tested on Spike RISC-V ISA Simulator, qemu RISC-V System emulator, and gem5 simulator.

Master branch holds the most updated version (rvv-v1.0) compatible with latest instrinsics andd toolchain. Additionaly, there are two more available versions:
[rvv-0.7](https://github.com/RALC88/riscv-vectorized-benchmark-suite/tree/rvv-0.7) which is based on  RISC-V V vector extension v0.7. This version can be only compiled by the LLVM from Barcelona Supercomputing Center. Instrinsics also corresponds to that specific compiler.
[rvv-1.0-PLCTLab](https://github.com/RALC88/riscv-vectorized-benchmark-suite/tree/rvv-1.0-PLCTLab) which is based on  RISC-V V vector extension v1.0. This version can be only compiled by the LLVM from PLCT. Instrinsics also corresponds to that specific compiler.


If you use this software or a modified version of it for your research, please cite the paper:
Cristóbal Ramírez, César Hernandez, Oscar Palomar, Osman Unsal, Marco Ramírez, and Adrián Cristal. 2020. A RISC-V Simulator and Benchmark Suite for Designing and Evaluating Vector Architectures. ACM Trans. Archit. Code Optim. 17, 4, Article 38 (October 2020), 29 pages. https://doi.org/10.1145/3422667

## Vectorized apps

| Application Name  | Application Domain            | Algorithmic Model     | Taken From  |
| ----------------- |------------------------------ | --------------------- | ----------- |
| Axpy              | High Performance Computing    | BLAS                  | -           |
| Blackscholes      | Financial Analysis            | Dense Linear Algebra  | PARSEC      |
| Canneal           | Engineering                   | Unstructured Grids    | PARSEC      |
| LavaMD2           | Molecular Dynamics            | N-Body                | Rodinia     |
| matmul            | High Performance Computing    | BLAS                  | -           |
| Particle Filter   | Medical Imaging               | Structured Grids      | Rodinia     |
| Somier            | Physics Simulation            | Dense Linear Algebra  | -           |
| Jacobi-2D         | Engineering                   | Dense Linear Algebra  | PolyBench   |
| Pathfinder        | Grid Traversal                | Dynamic Programming   | Rodinia     |
| Streamcluster     | Data Mining                   | Dense Linear Algebra  | PARSEC      |
| Swaptions         | Financial Analysis            | MapReduce Regular     | PARSEC      |


## Building the rv64gcv baremetal toolchain with LLVM

To compile the suite it is required to install the [RISC-V GNU Compiler Toolchain] (https://github.com/riscv-collab/riscv-gnu-toolchain). Next lines summarizes the commands to build the rv64gcv baremetal toolchain with LLVM. We strongly recommend to look at the official toolchain repository for more details.


Set the installation PATH 
```
export RISCV="/your_path/build/riscv"
export PATH="$RISCV:$PATH"
```

Cloning the repository 
```
git clone https://github.com/riscv/riscv-gnu-toolchain
```

Install dependencies
```
sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev
```

Install RISC-V GNU Compiler Toolchain 
```
cd riscv-gnu-toolchain
./configure --prefix=$RISCV --enable-llvm --disable-linux --with-arch=rv64gcv --with-cmodel=medany
make -j37 all build-sim SIM=qemu
```


### The RISC-V vector C intrinsics 

As mentioned above, RiVEC implements the lastest riscv intrinsics for rvv-1.0.  RISC-V vector C intrinsics provide users interfaces in the C language level to directly leverage the RISC-V "V" extension.  More details can be found [here] (https://github.com/riscv-non-isa/rvv-intrinsic-doc/blob/main/doc/rvv-intrinsic-spec.adoc).

Here a nice instrinsic viewer tool https://dzaima.github.io/intrinsics-viewer

### Setting up the environment

The Suite includes a makefile to compile every application, in order to use it, you must define the path to the RISC-V vector compiler.

Setting the Vector Compiler path
```
export RISCV="/your_path/build/riscv"
export PATH="$RISCV:$PATH"
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
This will compile two versions of the application, namely serial and vector versions.

Also you can compile all the applications by typing:
```
make all 
```

## Running the Vectorized apps

To be able to run all the apps, you must install Spike, qemu, and/or gem5. All the applications run succesfully in spike and gem5. Applications that does not require an input file, or generate an output file are able to run on qemu as well.

### Installing Spike, qemu, and gem5

All the RiVEC applications can run on [Spike] (https://github.com/riscv-software-src/riscv-isa-sim).  We strongly recommend to look at the official Spike RISC-V ISA Simulator repository for more details.

Install Spike RISC-V ISA Simulator
```
sudo apt-get install device-tree-compiler libboost-regex-dev libboost-system-dev
git clone https://github.com/riscv-software-src/riscv-isa-sim.git
cd riscv-isa-sim/
mkdir build
cd build/
../configure --prefix=$RISCV --with-arch=rv64gcv
make -j17
make install
```

### Installing Qemu

When building the rv64gcv baremetal toolchain with LLVM with the above provided commands, it was indicated to also install QEMU

### Running the apps

The suite provides an interactive script (run.sh) which provides a simple way to execute every app.

First run the following command:
```
source run.sh
```
That will display all the applications available, next it ask if do you want to run the serial or vectorized version, followed by the simulation size. Some applications creates an output file located in the folder "output" inside each application folder.


Running Axpy:
```
source run.sh

----------------------------------------------------------------------------------
RiVec Benchmark Suite
----------------------------------------------------------------------------------

select one application to run [axpy blackscholes canneal jacobi-2d lavaMD matmul 
swaptions streamcluster somier particlefilter pathfinder]: axpy
 
----------------------------------------------------------------------------------
AXPY
----------------------------------------------------------------------------------
 
do you want to run the serial or vectorized version [serial vector]: vector
select the simulation size [tiny small medium large]: tiny
 
----------------------------------------------------------------------------------
RUNNING AXPY
----------------------------------------------------------------------------------
command: /home/ralc/Desktop/RISC-V/build/riscv/bin/spike --isa=rv64gcv pk bin/axpy_vector.exe 256
----------------------------------------------------------------------------------
init_vector time: 0.002524
doing reference axpy , vector size 262144
axpy_serial time: 0.000525
doing vector axpy, vector size 262144
axpy_intrinsics time: 0.001442
done
Result ok !!!

```

### Simulation sizes 

There are provided 4 different simulation sizes (arguments to run the application).

Whe you are executing an application, it will include the corresopnding argument to run a predefined simsize. The interactive script already includes all those simulation sizes.
#### simtiny 
```
blackscholes_args   = "1 input/in_512.input output_prices.txt"
canneal_args        = "1 100 300 input/100.nets 8"
streamcluster_args  = "3 10 128 128 128 10 none output.txt 1"
swaptions_args      = "-ns 8 -sm 512 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 2 -np 256"
pathfinder_args     = "_pathfinder/input/data_tiny.in"
axpy_args           = "256"
lavaMD_args         = "-cores 1 -boxes1d 1 -outputFile result.txt"
somier_args         = "2 64"
jacobi_2d_args      = "32 2 output.txt"
```

#### simsmall 
```
blackscholes_args   = "1 input/in_4K.input output_prices.txt"
canneal_args        = "1 10000 2000 input/100000.nets 32"
streamcluster_args  = "10 20 128 4096 4096 1000 none output.txt 1"
swaptions_args      = "-ns 8 -sm 4096 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 8 -np 1024"
pathfinder_args     = "_pathfinder/input/data_small.in"
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
pathfinder_args     = "_pathfinder/input/data_medium.in""
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
pathfinder_args     = ""_pathfinder/input/data_large.in""
axpy_args           = "2048"
lavaMD_args         = "-cores 1 -boxes1d 10 -outputFile result.txt"
somier_args         = "4 128"
jacobi_2d_args      = "256 2000 output.txt"
```  

## Contributing

New vectorized applications are welcome. We strongly encourage you to contribute with new apps to enrich the scenarios and diversity in the RiVEC Bencmark Suite.

## Acknowledgements
We thank the [European Processor Initiative](https://www.european-processor-initiative.eu/) (EPI) project, and the Barcelona Supercomputing Center compiler team, specially to Roger Ferrer who have always support us and solved our doubts about compiler-related issues.

## Contact
Cristobal Ramirez
cristobal.ramirez.lazo@gmail.com
