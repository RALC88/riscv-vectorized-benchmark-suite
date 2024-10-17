#makefile

start:
	rm bin/*.*

serial: # with -O2 fails in spike and gem5 the vector version
	${RISCV}/bin/clang++ -march=rv64g -fno-vectorize -c -o src/blackscholes_serial.o src/blackscholes.cpp
	${RISCV}/bin/clang++ -march=rv64g -fno-vectorize -o bin/blackscholes_serial.exe src/blackscholes_serial.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump --mattr=+m,+f,+d,+a,+c -ds  bin/blackscholes_serial.exe > bin/blackscholes_serial.dump

vector: # with -O2 fails in spike and gem5 
	${RISCV}/bin/clang++  -march=rv64gcv -DUSE_RISCV_VECTOR  -fno-vectorize -c -o src/blackscholes.o src/blackscholes.cpp ; \
	${RISCV}/bin/clang++  -march=rv64gcv -DUSE_RISCV_VECTOR  -fno-vectorize -o bin/blackscholes_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${RISCV}/bin/llvm-objdump --mattr=+m,+f,+d,+a,+c,+v -ds bin/blackscholes_vector.exe > bin/blackscholes_vector.dump ;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;