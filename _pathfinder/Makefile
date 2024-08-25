#makefile

start:
	rm -f bin/*.*

serial:
	${RISCV}/bin/clang++ -march=rv64g -O2 -o bin/pathfinder_serial.exe src/pathfinder.cpp
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/pathfinder_serial.exe > bin/pathfinder_serial.dump

vector:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -mepi -DUSE_RISCV_VECTOR -march=rv64g -O2 -fno-vectorize -c -o src/pathfinder.o src/pathfinder.cpp ; \
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -mepi -DUSE_RISCV_VECTOR -march=rv64g -O2 -fno-vectorize -o bin/pathfinder_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds   bin/pathfinder_vector.exe > bin/pathfinder_vector.dump ; \

clean:
	rm -f bin/*.dump; \
	rm -f bin/*.exe;
