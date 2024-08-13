#makefile

start:
	rm bin/*.*

serial:
	echo "serial"
	${RISCV}/bin/clang -march=rv64gcv -O2 -c -o src/matrixmul.o src/matrixmul.c
	${RISCV}/bin/clang -march=rv64gcv -O2 -c -o src/main.o src/main.c
	${RISCV}/bin/clang -march=rv64gcv -O2 -o bin/matrixmul_serial.exe src/*.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds  bin/matrixmul_serial.exe > bin/matrixmul_serial.dump

vector:
	echo "vector"
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -c -o src/matrixmul.o src/matrixmul.c
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -c -o src/main.o src/main.c
	${RISCV}/bin/clang -march=rv64gcv -O2 -o bin/matrixmul_vector.exe src/*.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds  bin/matrixmul_vector.exe > bin/matrixmul_vector.dump

clean:
	rm bin/*.dump; \
	rm bin/*.exe;