#makefile

start:
	mkdir bin; \
	rm bin/*.*

serial:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -c -o src/blackscholes_serial.o src/blackscholes.cpp
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -o bin/blackscholes_serial.exe src/blackscholes_serial.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/blackscholes_serial.exe > bin/blackscholes_serial.dump
vector:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/blackscholes.o src/blackscholes.cpp ; \
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -o bin/blackscholes_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds   bin/blackscholes_vector.exe > bin/blackscholes_vector.dump ;

host:
	gcc -O2 -fopenmp src/blackscholes.cpp -o bin/blackscholes_host.exe -lm ;

copy:
	rm -r ../../Test/_blackscholes/bin/*.exe; \
	cp -r bin/*exe ../../Test/_blackscholes/bin;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;