#makefile

start:
	rm -f bin/*.*

serial:
	${RISCV}/bin/clang++ -DRESULT_PRINT -DINPUT_PRINT -march=rv64g -O2 -o bin/pathfinder_serial.exe src/pathfinder.cpp
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/pathfinder_serial.exe > bin/pathfinder_serial.dump

vector:
	${RISCV}/bin/clang++ -DUSE_RISCV_VECTOR -DRESULT_PRINT -DINPUT_PRINT -march=rv64gcv -O2 -I../common -o bin/pathfinder_vector.exe src/pathfinder.cpp  
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+v -ds   bin/pathfinder_vector.exe > bin/pathfinder_vector.dump 

clean:
	rm -f bin/*.dump; \
	rm -f bin/*.exe;
