#makefile

start:
	rm bin/*.*;

serial:
	${RISCV}/bin/clang -march=rv64g -O3 -fno-vectorize -c -o num.o  util/num/num.c ; \
	${RISCV}/bin/clang -march=rv64g -O3 -fno-vectorize -c -o kernel_cpu.o  kernel/kernel_cpu.c ; \
	${RISCV}/bin/clang -march=rv64g -O3 -fno-vectorize -c -o main.o  main.c ; \
	${RISCV}/bin/clang -march=rv64g -O3 -fno-vectorize -o bin/lavaMD_serial.exe *.o -lm ; \
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/lavaMD_serial.exe > bin/lavaMD_serial.dump ; \
	rm *.o ; \

vector:
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O3 -fno-vectorize -c -o num.o  util/num/num.c ; \
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O3 -fno-vectorize -c -o kernel_vector.o  kernel/kernel_vector.c ; \
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O3 -fno-vectorize -c -o main.o  main.c ; \
	${RISCV}/bin/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O3 -fno-vectorize -o bin/lavaMD_vector.exe *.o -lm ; \
	${RISCV}/bin/llvm-objdump  --mattr=+m,+f,+d,+a,+c -ds  bin/lavaMD_vector.exe > bin/lavaMD_vector.dump ; \
	rm *.o ; \

clean:
	rm bin/*.dump; \
	rm bin/*.exe;


