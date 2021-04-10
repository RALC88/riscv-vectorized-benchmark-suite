
CC=${LLVM}/bin/clang
OBJDUMP=${LLVM}/bin/llvm-objdump

CFLAGS += -O2 -mepi -mabi=lp64d

LDFLAGS = -lm -static
IFLAGS = -I ./ -I ${VEHAVE_TOOLCHAIN}/include/vehave/ -I ${VEHAVE_TOOLCHAIN}/include/vehave-user/

.PHONY: all all-linux

all: all-linux

all-linux: somier_serial.exe somier_vector.exe somier_vector_lmul2.exe somier_vector_lmul4.exe somier_vector_lmul8.exe somier_vector_rva.exe

start:
	rm *.exe
	rm *.dump

serial: somier_serial.exe somier_serial.dump
vector: somier_vector.exe somier_vector.dump

somier_serial.exe: main.seq.o somier.o utils.o forces.o
	$(CC) -O2 -o $@ $+ $(LDFLAGS)

somier_vector.exe: main.vec.o somier.o utils.o forces.o forces_prevec_lmul1.o somier_intr_lmul1.o
	$(CC) -O2 -o $@ $+ $(LDFLAGS)

main.seq.o: main.c utils.h
	$(CC) $(CFLAGS) $(IFLAGS) -DSEQ -c -o $@ $<

main.vec.o: main.c utils.h
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

somier.o: omp/somier.c utils.h
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

somier_intr_lmul1.o: intrinsics/somier_intr_lmul1.c intrinsics/somier_v.h utils.h
	$(CC) $(CFLAGS) -DUSE_RISCV_VECTOR $(IFLAGS) -c -o $@ $<

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o $@ $<

forces.o: forces.c utils.h
	$(CC) $(CFLAGS) -c -o $@ $<

forces_prevec_lmul1.o: intrinsics/forces_prevec_lmul1.c utils.h
	$(CC) $(CFLAGS) -DUSE_RISCV_VECTOR $(IFLAGS) -c -o $@ $<

#crear_dump: %.dump: %.exe
#		${OBJDUMP} --mattr=+m,+f,+d,+a,+c -ds   $< > $@

somier_serial.dump:
	${OBJDUMP} --mattr=+m,+f,+d,+a,+c -ds  somier_serial.exe > somier_serial.dump

somier_vector.dump:
	${OBJDUMP} --mattr=+m,+f,+d,+a,+c,+experimental-v -ds  somier_vector.exe > somier_vector.dump

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.exe
