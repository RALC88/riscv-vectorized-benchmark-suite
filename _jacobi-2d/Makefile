#makefile
GCC_TOOLCHAIN_DIR := /newlib/
SYSROOT_DIR := $(GCC_TOOLCHAIN_DIR)/riscv64-unknown-elf

LLVM := /llvm/build/
SPIKE := spike
PK := pk

target = bin/jacobi-2d_vector.exe

start:
	mkdir -p bin; \
	rm bin/*.*
	
serial:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -O2 -c -o src/jacobi-2d.o src/jacobi-2d.cpp; \
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf  -march=rv64g -O2 -o bin/jacobi-2d_serial.exe src/*.o -lm; \
	rm src/*.o; \
	${LLVM}/bin/llvm-objdump --mattr=+v -ds  bin/jacobi-2d_serial.exe > bin/jacobi-2d_serial.dump

vector:
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/jacobi-2d.o src/jacobi-2d.cpp; \
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -o bin/jacobi-2d_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${LLVM}/bin/llvm-objdump --mattr=+v -ds  bin/jacobi-2d_vector.exe > bin/jacobi-2d_vector.dump ; \

runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) 64 100 jacobi_2d.out

host:
	g++ -O2 -g -Wall -std=c99 -c -o  src/jacobi-2d.o  src/jacobi-2d.cpp ; \
	g++ -O2 -g -Wall -std=c99 -o  bin/jacobi-2d_host.exe  src/*.o  -lm; \
	rm src/*.o; \
	objdump -dC bin/jacobi-2d_host.exe > bin/jacobi-2d_host.dump ;

copy:
	rm -r ../../Test/_jacobi-2d/bin/*.exe; \
	cp -r bin/*exe ../../Test/_jacobi-2d/bin/;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;