#makefile
GCC_TOOLCHAIN_DIR := /newlib/
SYSROOT_DIR := $(GCC_TOOLCHAIN_DIR)/riscv64-unknown-elf

LLVM := /llvm/build/
SPIKE := spike
PK := pk

target = bin/rvv-test

start:
	mkdir bin; \
	rm bin/*.*

serial:
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf   -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/particlefilter_serial.o src/particlefilter.c
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf  -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -o $(target) src/particlefilter_serial.o -lm
	rm src/*.o

vector:
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -DMVL_$$mvl   -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -c -o src/particlefilter.o src/particlefilter.c ; \
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -DMVL_$$mvl -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -o $(target) src/*.o -lm ; \
	rm src/*.o 

runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) -x 128 -y 128 -z 2 -np 256


clean:
	rm bin/*

	
