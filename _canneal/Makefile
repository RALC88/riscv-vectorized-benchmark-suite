#makefile

start:
	rm bin/*.*;

serial:
	for entry in src/*.cpp ; do \
		${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -O2 -fno-vectorize -c -o $$entry.o  $$entry ; \
	done
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -O2 -fno-vectorize -o bin/canneal_serial.exe src/*.cpp.o -lm ; \
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/canneal_serial.exe > bin/canneal_serial.dump ; \
	rm src/*.o ; \

vector:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/annealer_thread.o src/annealer_thread.cpp; \
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/main.o src/main.cpp; \
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/netlist.o src/netlist.cpp; \
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/netlist_elem.o src/netlist_elem.cpp; \
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/rng.o src/rng.cpp; \
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize  -o bin/canneal_vector.exe src/*.o -lm; \
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds  bin/canneal_vector.exe > bin/canneal_vector.dump ; \
	rm src/*.o;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;