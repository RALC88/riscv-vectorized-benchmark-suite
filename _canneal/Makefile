#makefile

start:
	mkdir bin ; \
	rm bin/*.*;

serial:
	for entry in src/*.cpp ; do \
		${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -O2 -c -o  $$entry.o  $$entry ; \
	done
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -O2  -o bin/canneal_serial.exe src/*.cpp.o -lm;	\
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c -ds  bin/canneal_serial.exe > bin/canneal_serial.dump;	\
	rm src/*.o; \

vector:
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/annealer_thread.o src/annealer_thread.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/main.o src/main.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/netlist.o src/netlist.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/netlist_elem.o src/netlist_elem.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2 -c -o src/rng.o src/rng.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64g -O2  -o bin/canneal_vector.exe src/*.o -lm;	\
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/canneal_vector.exe > bin/canneal_vector.dump ; \
	rm src/*.o;

host:
	for entry in src/*.cpp ; do \
		g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -c -o  $$entry.o  $$entry ; \
	done
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -o  bin/canneal_host.exe  src/*.o  -lm; \
	rm src/*.o;

copy:
	rm -r ../../Test/_canneal/bin/*.exe; \
	cp -r bin/*exe ../../Test/_canneal/bin/;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;