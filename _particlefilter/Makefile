#makefile

start:
	rm bin/*.*

serial:
	${RISCV}/bin/clang -march=rv64g -O2 -fno-vectorize -c -o src/particlefilter_serial.o src/particlefilter.c
	${RISCV}/bin/clang -march=rv64g -O2 -fno-vectorize -o bin/particlefilter_serial.exe src/particlefilter_serial.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump --mattr=+m,+f,+d,+a,+c -ds  bin/particlefilter_serial.exe > bin/particlefilter_serial.dump

vector:
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/particlefilter.o src/particlefilter.c
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize -o bin/particlefilter_vector.exe src/*.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+v -ds   bin/particlefilter_vector.exe > bin/particlefilter_vector.dump

clean:
	rm bin/*.dump
	rm bin/*.exe