#makefile

start:
	mkdir bin; \
	rm bin/*.*

serial:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -O2 -c -o src/pathfinder_serial.o src/pathfinder.cpp
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -o bin/pathfinder_serial.exe src/pathfinder_serial.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/pathfinder_serial.exe > bin/pathfinder_serial.dump

vector:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/pathfinder.o src/pathfinder.cpp ; \
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -o bin/pathfinder_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds   bin/pathfinder_vector.exe > bin/pathfinder_vector.dump ; \

host:
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -c -o  src/pathfinder.o  src/pathfinder.cpp ; \
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -o  bin/pathfinder_host.exe  src/*.o  -lm; \
	rm src/*.o;

copy:
	rm -r ../../Test/_pathfinder/bin/*.exe; \
	cp -r bin/*exe ../../Test/_pathfinder/bin/;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;