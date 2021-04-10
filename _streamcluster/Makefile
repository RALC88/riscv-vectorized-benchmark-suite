#makefile

start:
	rm bin/*.*

serial:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -O2 -fno-vectorize -c -o src/streamcluster_serial.o src/streamcluster.cpp
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -O2 -fno-vectorize -o bin/streamcluster_serial.exe src/streamcluster_serial.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/streamcluster_serial.exe > bin/streamcluster_serial.dump

vector:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -mepi -DUSE_RISCV_VECTOR -march=rv64g -O2 -fno-vectorize -c -o src/streamcluster.o src/streamcluster.cpp
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -mepi -DUSE_RISCV_VECTOR -march=rv64g -O2 -fno-vectorize -o bin/streamcluster_vector.exe src/*.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds   bin/streamcluster_vector.exe > bin/streamcluster_vector.dump

clean:
	rm bin/*.dump
	rm bin/*.exe