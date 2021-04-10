#makefile

start:
	rm bin/*.*

serial:
	${LLVM}/bin/clang --target=riscv64-unknown-elf -mepi -march=rv64g -O2 -fno-vectorize -c -o src/particlefilter_serial.o src/particlefilter.c
	${LLVM}/bin/clang --target=riscv64-unknown-elf -mepi -march=rv64g -O2 -fno-vectorize -o bin/particlefilter_serial.exe src/particlefilter_serial.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump --mattr=+m,+f,+d,+a,+c -ds  bin/particlefilter_serial.exe > bin/particlefilter_serial.dump

vector:
	${LLVM}/bin/clang --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/particlefilter.o src/particlefilter.c
	${LLVM}/bin/clang --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -o bin/particlefilter_vector.exe src/*.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds   bin/particlefilter_vector.exe > bin/particlefilter_vector.dump

clean:
	rm bin/*.dump
	rm bin/*.exe
	