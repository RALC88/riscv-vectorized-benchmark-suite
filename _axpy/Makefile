#makefile

start:
	rm bin/*.*

serial:
	echo "serial"
	${RISCV}/bin/clang -march=rv64g -O2 -c -o src/utils.o src/utils.c
	${RISCV}/bin/clang -march=rv64g -O2 -c -o src/axpy.o src/axpy.c
	${RISCV}/bin/clang -march=rv64g -O2 -c -o src/main.o src/main.c
	${RISCV}/bin/clang -march=rv64g -O2 -o bin/axpy_serial.exe src/*.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump --mattr=+m,+f,+d,+a,+c -ds  bin/axpy_serial.exe > bin/axpy_serial.dump

vector:
	echo "vector"
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR  -fno-vectorize -O2 -c -o src/axpy.o src/axpy.c
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR  -fno-vectorize -c -o src/main.o src/main.c
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR  -fno-vectorize -O2 -c -o src/utils.o src/utils.c
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR  -fno-vectorize -O2 -o bin/axpy_vector.exe src/*.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+v -ds  bin/axpy_vector.exe > bin/axpy_vector.dump

clean:
	rm bin/*.dump; \
	rm bin/*.exe;