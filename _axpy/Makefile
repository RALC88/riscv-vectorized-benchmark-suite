#makefile
TEST_DIR := $HOME/plct-machine
GCC_TOOLCHAIN_DIR := $(TEST_DIR)/riscv
SYSROOT_DIR := $(GCC_TOOLCHAIN_DIR)/riscv64-unknown-elf

LLVM := $(TEST_DIR)/rvv-llvm/build/install
QEMU := $(TEST_DIR)/build-64/qemu-riscv64
SPIKE := spike
PK := pk

target = bin/rvv-test

serial:
	${LLVM}/bin/clang  -Wall --target=riscv64-unknown-elf -march=rv64gcv1p0 -menable-experimental-extensions -O2  --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -c -o src/axpy.o src/axpy.c
	${LLVM}/bin/clang  -Wall --target=riscv64-unknown-elf -march=rv64gcv1p0 -menable-experimental-extensions -O2  --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -c -o src/main.o src/main.c
	${LLVM}/bin/clang  -Wall --target=riscv64-unknown-elf  -march=rv64gcv1p0 -menable-experimental-extensions -O2  --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -c -o src/utils.o src/utils.c
	${LLVM}/bin/clang  -Wall --target=riscv64-unknown-elf -march=rv64gcv1p0 -menable-experimental-extensions -O2  --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -o $(target) src/*.o -lm

vector:
	${LLVM}/bin/clang  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR  -march=rv64gcv1p0 -menable-experimental-extensions -O2  --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -c -o src/utils.o src/utils.c
	${LLVM}/bin/clang  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions -O2  --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -c -o src/axpy.o src/axpy.c
	${LLVM}/bin/clang  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR  -march=rv64gcv1p0 -menable-experimental-extensions -O2  --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -c -o src/main.o src/main.c
	${LLVM}/bin/clang  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions -O2  --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -o $(target) src/*.o -lm  
	
runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) 256

runqemu:
	$(QEMU) -cpu plct-u64,x-v=true $(target) 256