
CC=${RISCV}/bin/clang
OBJDUMP=${RISCV}/bin/llvm-objdump

CFLAGS += -march=rv64g -O2 -fno-vectorize
CFLAGS_VECTOR += -march=rv64gcv -O2 -fno-vectorize

LDFLAGS = -lm -static
IFLAGS = -I ./ -I ${VEHAVE_TOOLCHAIN}/include/vehave/ -I ${VEHAVE_TOOLCHAIN}/include/vehave-user/

start:
	rm -f bin/*.*

serial: bin/somier_serial.exe bin/somier_serial.dump clean_o
vector: bin/somier_vector.exe bin/somier_vector.dump clean_o

bin/somier_serial.exe: main.seq.o somier.o utils.o forces.o
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

bin/somier_vector.exe: main.vec.o somier.o utils.o forces_prevec_lmul1.o somier_intr_lmul1.o
	$(CC) $(CFLAGS_VECTOR) -o $@ $+ $(LDFLAGS)

main.seq.o: main.c utils.h
	$(CC) $(CFLAGS) $(IFLAGS) -DSEQ -c -o $@ $<

main.vec.o: main.c utils.h
	$(CC) $(CFLAGS_VECTOR) $(IFLAGS) -c -o $@ $<

somier.o: omp/somier.c utils.h
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

somier_intr_lmul1.o: intrinsics/somier_intr_lmul1.c intrinsics/somier_v.h utils.h
	$(CC) $(CFLAGS_VECTOR) -DUSE_RISCV_VECTOR $(IFLAGS) -c -o $@ $<

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o $@ $<

forces.o: forces.c utils.h
	$(CC) $(CFLAGS) -c -o $@ $<

forces_prevec_lmul1.o: intrinsics/forces_prevec_lmul1.c utils.h
	$(CC) $(CFLAGS_VECTOR) -DUSE_RISCV_VECTOR $(IFLAGS) -c -o $@ $<

bin/somier_serial.dump:
	${OBJDUMP} --mattr=+m,+f,+d,+a,+c -ds  bin/somier_serial.exe > bin/somier_serial.dump

bin/somier_vector.dump:
	${OBJDUMP} --mattr=+m,+f,+d,+a,+c,+v -ds  bin/somier_vector.exe > bin/somier_vector.dump

clean_o:
	rm -f *.o

clean:
	rm -f *.o
	rm -f *.exe
