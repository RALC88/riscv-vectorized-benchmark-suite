#makefile
GCC_TOOLCHAIN_DIR := /home/liaochunyu/cxo/newlib/
SYSROOT_DIR := $(GCC_TOOLCHAIN_DIR)/riscv64-unknown-elf

LLVM := /home/liaochunyu/rvv-llvm/build/
SPIKE := /home/liaochunyu/riscv-isa-sim/build/spike
PK := /home/liaochunyu/riscv-pk/build/pk

target = bin/rvv-test

start:
	mkdir bin ; \
	rm bin/*.*;

serial:
	for entry in src/*.cpp ; do \
		${LLVM}/bin/clang++  -Wall --target=riscv64-unknown-elf  -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -c -o  $$entry.o  $$entry ; \
	done
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf  -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2  -o $(target) src/*.cpp.o -lm
	rm src/*.o

x86:
	for entry in src/*.cpp ; do \
        ${LLVM}/bin/clang++  -Wall  -O2 -c -o  $$entry.o  $$entry ; \
    done
	${LLVM}/bin/clang++  -Wall  -O2  -o $(target) src/*.cpp.o -lm
	$(target) 1 100 300 input/100.nets 8
	rm src/*.o

vector:
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR  -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/annealer_thread.o src/annealer_thread.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/main.o src/main.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/netlist.o src/netlist.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/netlist_elem.o src/netlist_elem.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/rng.o src/rng.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2  -o bin/canneal_vector.exe src/*.o -lm
	rm src/*.o

runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) 1 100 300 input/100.nets 8

host:
	for entry in src/*.cpp ; do \
		g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -c -o  $$entry.o  $$entry ; \
	done
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -o  bin/canneal_host.exe  src/*.o  -lm; \
	rm src/*.o;

copy:
	rm -r ../../Test/_canneal/bin/*.exe; \
	cp -r bin/*exe ../../Test/_canneal/bin/;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;
