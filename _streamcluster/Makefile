#makefile

start:
	rm bin/*.*

serial:
	${RISCV}/bin/clang++ -march=rv64g -O2 -fno-vectorize -c -o src/streamcluster_serial.o src/streamcluster.cpp
	${RISCV}/bin/clang++ -march=rv64g -O2 -fno-vectorize -o bin/streamcluster_serial.exe src/streamcluster_serial.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/streamcluster_serial.exe > bin/streamcluster_serial.dump

vector:
	${RISCV}/bin/clang++ -DUSE_RISCV_VECTOR -march=rv64gcv -O2 -fno-vectorize -c -o src/streamcluster.o src/streamcluster.cpp
	${RISCV}/bin/clang++ -DUSE_RISCV_VECTOR -march=rv64gcv -O2 -fno-vectorize -o bin/streamcluster_vector.exe src/*.o -lm
	rm src/*.o
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+v -ds   bin/streamcluster_vector.exe > bin/streamcluster_vector.dump

clean:
	rm bin/*.dump
	rm bin/*.exe