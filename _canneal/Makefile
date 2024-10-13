#makefile

start:
	rm bin/*.*;

serial:
	for entry in src/*.cpp ; do \
		${RISCV}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -O2 -fno-vectorize -c -o $$entry.o  $$entry ; \
	done
	${RISCV}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -O2 -fno-vectorize -o bin/canneal_serial.exe src/*.cpp.o -lm ; \
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/canneal_serial.exe > bin/canneal_serial.dump ; \
	rm src/*.o ; \

vector:
	${RISCV}/bin/clang++ -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/annealer_thread.o src/annealer_thread.cpp; \
	${RISCV}/bin/clang++ -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/main.o src/main.cpp; \
	${RISCV}/bin/clang++ -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/netlist.o src/netlist.cpp; \
	${RISCV}/bin/clang++ -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/netlist_elem.o src/netlist_elem.cpp; \
	${RISCV}/bin/clang++ -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/rng.o src/rng.cpp; \
	${RISCV}/bin/clang++ -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -fno-vectorize  -o bin/canneal_vector.exe src/*.o -lm; \
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+v -ds  bin/canneal_vector.exe > bin/canneal_vector.dump ; \
	rm src/*.o;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;