#makefile

start:
	rm -f bin/*.*

serial:
	echo "serial"
	${RISCV}/bin/clang -march=rv64gcv -O2 -o bin/matrixmul_serial.exe src/matmul.c src/main.c
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/matrixmul_serial.exe > bin/matrixmul_serial.dump

vector:
	echo "vector"
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -o bin/matrixmul_vector.exe src/matmul.c src/main.c
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+v -S  bin/matrixmul_vector.exe > bin/matrixmul_vector.dump

clean:
	rm -f bin/*.dump; \
	rm -f bin/*.exe;
