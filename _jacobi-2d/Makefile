#makefile

start:
	rm bin/*.*
	
serial:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -O2 -fno-vectorize -c -o src/jacobi-2d.o src/jacobi-2d.cpp
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -O2 -fno-vectorize -o bin/jacobi-2d_serial.exe src/*.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/jacobi-2d_serial.exe > bin/jacobi-2d_serial.dump

vector:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -mepi -O2 -fno-vectorize -c -o src/jacobi-2d.o src/jacobi-2d.cpp
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -mepi -O2 -fno-vectorize -o bin/jacobi-2d_vector.exe src/*.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds   bin/jacobi-2d_vector.exe > bin/jacobi-2d_vector.dump

clean:
	rm bin/*.dump
	rm bin/*.exe