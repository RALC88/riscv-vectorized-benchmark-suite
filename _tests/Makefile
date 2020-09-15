#makefile

start:
	mkdir bin; \
	rm bin/*.*

serial:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -c -o src/tests.o src/tests.cpp
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -o bin/tests_serial.exe src/tests_serial.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/tests_serial.exe > bin/tests_serial.dump
vector:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/tests.o src/tests.cpp ; \
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -o bin/tests_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds   bin/tests_vector.exe > bin/tests_vector.dump ;

host:
	gcc -O2 -fopenmp src/tests.cpp -o bin/tests_host.exe -lm ;

copy:
	rm -r ../../Test/_tests/bin/*.exe; \
	cp -r bin/*exe ../../Test/_tests/bin;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;