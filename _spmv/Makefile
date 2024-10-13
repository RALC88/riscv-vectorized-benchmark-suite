start:
	rm -f bin/*.*

serial:
	echo "serial"
	${RISCV}/bin/clang -march=rv64gc -O2 -o bin/spmv_serial.exe src/spmv.c src/main.c
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/spmv_serial.exe > bin/spmv_serial.dump

vector:
	echo "vector"
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -I../common -o bin/spmv_vector.exe src/spmv.c src/main.c
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c,+v -S  bin/spmv_vector.exe > bin/spmv_vector.dump

clean:
	rm -f bin/*.dump; \
	rm -f bin/*.exe;
