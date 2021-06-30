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
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -o $(target) src/blackscholes.cpp -lm

vector:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR   -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -o $(target)  src/blackscholes.cpp -lm


runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) 1 ./input/in_64K.input prices.txt

runqemu:
	$(QEMU) -cpu plct-u64,x-v=true $(target) 1 ./input/in_64K.input prices.txt
