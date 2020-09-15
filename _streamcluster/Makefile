#makefile

start:
	mkdir bin; \
	rm bin/*.*

serial:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -O2 -c -o src/streamcluster_serial.o src/streamcluster.cpp
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -o bin/streamcluster_serial.exe src/streamcluster_serial.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/streamcluster_serial.exe > bin/streamcluster_serial.dump

vector:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/streamcluster.o src/streamcluster.cpp ; \
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -o bin/streamcluster_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds   bin/streamcluster_vector.exe > bin/streamcluster_vector.dump ;

host:
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -c -o  src/streamcluster.o  src/streamcluster.cpp ; \
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -o  bin/streamcluster_host.exe  src/*.o  -lm; \
	rm src/*.o;

copy:
	rm -r ../../Test/_streamcluster/bin/*.exe; \
	cp -r bin/*exe ../../Test/_streamcluster/bin/;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;