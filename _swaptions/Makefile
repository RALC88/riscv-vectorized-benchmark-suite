#makefile

start:
	mkdir bin; \
	rm bin/*.*

serial:
	for entry in src/*.cpp ; do \
		${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -O2 -fno-vectorize -c -o  $$entry.o  $$entry ; \
	done
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -O2 -fno-vectorize -o bin/swaptions_serial.exe src/*.cpp.o -lm;	\
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/swaptions_serial.exe > bin/swaptions_serial.dump;	\
	rm src/*.o; \

vector:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/CumNormalInv.o src/CumNormalInv.cpp;	\
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/MaxFunction.o src/MaxFunction.cpp;	\
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/RanUnif.o src/RanUnif.cpp;	\
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/nr_routines.o src/nr_routines.cpp;	\
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/icdf.o src/icdf.cpp;	\
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/HJM_SimPath_Forward_Blocking.o src/HJM_SimPath_Forward_Blocking.cpp;	\
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/HJM.o src/HJM.cpp;	\
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/HJM_Swaption_Blocking.o src/HJM_Swaption_Blocking.cpp;	\
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -c -o src/HJM_Securities.o src/HJM_Securities.cpp;	\
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64g -mepi -DUSE_RISCV_VECTOR -O2 -fno-vectorize -o bin/swaptions_vector.exe src/*.o -lm;	\
	${LLVM}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+experimental-v -ds  bin/swaptions_vector.exe > bin/swaptions_vector.dump ; \
	rm src/*.o ;

clean:
	rm bin/*.dump
	rm bin/*.exe