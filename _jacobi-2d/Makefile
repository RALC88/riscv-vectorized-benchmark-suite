#makefile
LLVM := /llvm/build/

start:
	mkdir -p bin; \
	rm bin/*.*
	
serial:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -O2 -c -o src/jacobi-2d.o src/jacobi-2d.cpp; \
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf  -march=rv64g -O2 -o bin/jacobi-2d_serial.exe src/*.o -lm; \
	rm src/*.o; \
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/jacobi-2d_serial.exe > bin/jacobi-2d_serial.dump

vector:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/jacobi-2d.o src/jacobi-2d.cpp; \
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -o bin/jacobi-2d_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds   bin/jacobi-2d_vector.exe > bin/jacobi-2d_vector.dump ; \

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