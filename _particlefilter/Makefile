#makefile

start:
	mkdir bin; \
	rm bin/*.*

serial:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -O2 -c -o src/particlefilter_serial.o src/particlefilter.c
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -march=rv64g -O2 -o bin/particlefilter_serial.exe src/particlefilter_serial.o -lm
	rm src/*.o
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/particlefilter_serial.exe > bin/particlefilter_serial.dump

vector:
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -DMVL_$$mvl -O2 -c -o src/particlefilter.o src/particlefilter.c ; \
	${LLVM}/bin/clang++ -mepi -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -DMVL_$$mvl -march=rv64g -O2 -o bin/particlefilter_vector.exe src/*.o -lm ; \
	rm src/*.o ; \
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds   bin/particlefilter_vector.exe > bin/particlefilter_vector.dump ; \

host:
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -c -o  src/particlefilter.o  src/particlefilter.c ; \
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -o  bin/particlefilter_host.exe  src/*.o  -lm; \
	rm src/*.o;

copy:
	rm -r ../../Test/_particlefilter/bin/*.exe; \
	cp -r bin/*exe ../../Test/_particlefilter/bin/;

clean:
	rm bin/*.dump; \
	rm bin/*.exe;

	