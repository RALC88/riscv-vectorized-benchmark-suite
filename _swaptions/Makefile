#makefile

start:
	mkdir bin; \
	rm bin/*.*

serial:
	for entry in src/*.cpp ; do \
		${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -O2 -c -o  $$entry.o  $$entry ; \
	done
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -O2  -o bin/swaptions_serial.exe src/*.cpp.o -lm;	\
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/swaptions_serial.exe > bin/swaptions_serial.dump;	\
	rm src/*.o; \

vector:
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2 -c -o src/CumNormalInv.o src/CumNormalInv.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2 -c -o src/MaxFunction.o src/MaxFunction.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2 -c -o src/RanUnif.o src/RanUnif.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2 -c -o src/nr_routines.o src/nr_routines.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2 -c -o src/icdf.o src/icdf.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2 -c -o src/HJM_SimPath_Forward_Blocking.o src/HJM_SimPath_Forward_Blocking.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2 -c -o src/HJM.o src/HJM.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2 -c -o src/HJM_Swaption_Blocking.o src/HJM_Swaption_Blocking.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2 -c -o src/HJM_Securities.o src/HJM_Securities.cpp;	\
	${LLVM}/bin/clang++ -mepi  -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -O2  -o bin/swaptions_vector.exe src/*.o -lm;	\
	${LLVM}/bin/llvm-objdump  -mattr=+m,+f,+d,+a,+c,+epi -ds  bin/swaptions_vector.exe > bin/swaptions_vector.dump ; \
	rm src/*.o ; \

host:
	for entry in src/*.cpp ; do \
		g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -c -o  $$entry.o  $$entry ; \
	done
	g++ -O2 -g -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions -static-libgcc -o  bin/swaptions_host.exe  src/*.o  -lm;
	rm src/*.o;

copy:
	rm -r ../../Test/_swaptions/bin/*.exe; \
	cp -r bin/*exe ../../Test/_swaptions/bin/;

clean:
	rm bin/*.dump
	rm bin/*.exe