#makefile

start:
	rm bin/*.*

serial:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf  -march=rv64g -mepi -O2 -fno-vectorize -c -o src/blackscholes_serial.o src/blackscholes.cpp
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf  -march=rv64g -mepi -O2 -fno-vectorize -o bin/blackscholes_serial.exe src/blackscholes_serial.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump --mattr=+m,+f,+d,+a,+c -ds  bin/blackscholes_serial.exe > bin/blackscholes_serial.dump

vector:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf  -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/blackscholes.o src/blackscholes.cpp ; \
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf  -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -o bin/blackscholes_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${LLVM}/bin/llvm-objdump --mattr=+m,+f,+d,+a,+c,+experimental-v -ds bin/blackscholes_vector.exe > bin/blackscholes_vector.dump ;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;