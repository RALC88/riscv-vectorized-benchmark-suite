#makefile
GCC_TOOLCHAIN_DIR := /home/liaochunyu/cxo/newlib/
SYSROOT_DIR := $(GCC_TOOLCHAIN_DIR)/riscv64-unknown-elf

LLVM := /home/liaochunyu/rvv-llvm/build/
SPIKE := /home/liaochunyu/riscv-isa-sim/build/spike
PK := /home/liaochunyu/riscv-pk/build/pk

target = bin/rvv-test

start:
	mkdir bin; \
	rm bin/*.*

serial:
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf   -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/particlefilter_serial.o src/particlefilter.c
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf  -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -o $(target) src/particlefilter_serial.o -lm
	rm src/*.o


x86:
	${LLVM}/bin/clang   -Wall  -O2  -o $(target)  src/particlefilter.c -lm
	$(target)  -x 128 -y 128 -z 2 -np 256

vector:
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -DMVL_$$mvl   -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -c -o src/particlefilter.o src/particlefilter.c ; \
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -DMVL_$$mvl -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -o $(target) src/*.o -lm ; \
	rm src/*.o 

runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) -x 128 -y 128 -z 2 -np 256



host:
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -c -o  src/particlefilter.o  src/particlefilter.c ; \
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -o  bin/particlefilter_host.exe  src/*.o  -lm; \
	rm src/*.o;

copy:
	rm -r ../../Test/_particlefilter/bin/*.exe; \
	cp -r bin/*exe ../../Test/_particlefilter/bin/;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;

	
