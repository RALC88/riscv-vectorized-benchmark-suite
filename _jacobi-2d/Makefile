#makefile

start:
	rm bin/*.*
	
serial:
	${RISCV}/bin/clang++ -march=rv64g -O2 -fno-vectorize -c -o src/jacobi-2d.o src/jacobi-2d.cpp
	${RISCV}/bin/clang++ -march=rv64g -O2 -fno-vectorize -o bin/jacobi-2d_serial.exe src/*.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/jacobi-2d_serial.exe > bin/jacobi-2d_serial.dump

vector:
	${RISCV}/bin/clang++ -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o  src/jacobi-2d.o src/jacobi-2d.cpp
	${RISCV}/bin/clang++ -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize -o  bin/jacobi-2d_vector.exe src/*.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+v -ds   bin/jacobi-2d_vector.exe > bin/jacobi-2d_vector.dump

clean:
	rm bin/*.dump
	rm bin/*.exe