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
	${LLVM}/bin/clang --target=riscv64-unknown-elf -march=rv64g  -O2 -c -o src/utils.o src/utils.c
	${LLVM}/bin/clang --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -fno-vectorize -O2 -c -o src/axpy.o src/axpy.c
	${LLVM}/bin/clang --target=riscv64-unknown-elf -march=rv64g  -O2 -c -o src/main.o src/main.c
	${LLVM}/bin/clang --target=riscv64-unknown-elf -march=rv64g  -O2 -o bin/axpy_vector.exe src/*.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds  bin/axpy_vector.exe > bin/axpy_vector.dump

host:
	echo "host"
# 	gcc -O2 -g -Wall -std=c99 -c -o  src/axpy.o  src/axpy.c ; \
# 	gcc -O2 -g -Wall -std=c99 -c -o  src/main.o  src/main.c ; \
# 	gcc -O2 -g -Wall -std=c99 -c -o  src/utils.o  src/utils.c ; \
# 	gcc -O2 -g -Wall -std=c99 -o  bin/axpy_host.exe  src/*.o  -lm; \
# 	rm src/*.o; \
# 	objdump -dC bin/axpy_host.exe > bin/axpy_host.dump ;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;