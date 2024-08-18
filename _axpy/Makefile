#makefile

start:
	rm bin/*.*

serial:
	echo "serial"
#	${LLVM}/bin/clang -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -c -o src/axpy_serial.o src/axpy.c
#	${LLVM}/bin/clang -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -c -o src/main_serial.o src/main.c
#	${LLVM}/bin/clang -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -c -o src/utils_serial.o src/utils.c
#	${LLVM}/bin/clang -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -o bin/axpy_serial.exe src/*.o -lm
#	rm src/*.o
#	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/axpy_serial.exe > bin/axpy_serial.dump

vector:
	echo "vector"
	${RISCV}/bin/clang -march=rv64gcv -O2 -c -o src/utils.o src/utils.c
	${RISCV}/bin/clang -march=rv64gcv -O2 -c -o src/axpy.o src/axpy.c
	${RISCV}/bin/clang -march=rv64gcv -O2 -c -o src/main.o src/main.c
	${RISCV}/bin/clang -march=rv64gcv -O2 -o bin/axpy_vector.exe src/*.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds  bin/axpy_vector.exe > bin/axpy_vector.dump

clean:
	rm bin/*.dump; \
	rm bin/*.exe;