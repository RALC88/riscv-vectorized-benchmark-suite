# Example
# target: dependencies
	# command 1
	# command 2
          # .
          # .
          # .
	# command n
ifdef OUTPUT
override OUTPUT = -DOUTPUT
endif

#C_C = gcc
C_C = ${LLVM}/bin/clang
FLAGS = --target=riscv64-unknown-elf  -march=rv64g -mepi -O3 -fno-vectorize

dump = ${LLVM}/bin/llvm-objdump
OMP_LIB = #-lgomp
OMP_FLAG = #-fopenmp

start:
	rm bin/*.*

serial: main_serial.o ./kernel/kernel_cpu.o ./util/num/num.o ./util/timer/timer.o
	$(C_C) $(FLAGS) main.o ./kernel/kernel_cpu.o ./util/num/num.o ./util/timer/timer.o -lm -o bin/lavaMD_serial.exe ; \
	$(dump) --mattr=+m,+f,+d,+a,+c -ds  bin/lavaMD_serial.exe > bin/lavaMD_serial.dump ; \
	rm *.o ; \
	rm kernel/*.o ;

vector: main_vector.o ./kernel/kernel_vector.o ./util/num/num.o ./util/timer/timer.o
	$(C_C) $(FLAGS) -DUSE_RISCV_VECTOR main.o ./kernel/kernel_vector.o ./util/num/num.o ./util/timer/timer.o -lm -o bin/lavaMD_vector.exe ; \
	$(dump) --mattr=+m,+f,+d,+a,+c,+experimental-v -ds  bin/lavaMD_vector.exe > bin/lavaMD_vector.dump ; \
	rm *.o ; \
	rm kernel/*.o ;

# compile function files into objects (binaries)
main_serial.o:	main.h \
			main.c \
			./kernel/kernel_cpu.h \
			./kernel/kernel_cpu.c \
			./util/num/num.h \
			./util/num/num.c \
			./util/timer/timer.h \
			./util/timer/timer.c
	$(C_C) $(FLAGS) ${OUTPUT} main.c \
			-c \
			-o main.o \

main_vector.o:
	$(C_C) $(FLAGS) -DUSE_RISCV_VECTOR ${OUTPUT} main.c -c -o main.o

./kernel/kernel_cpu.o:	./kernel/kernel_cpu.h \
						./kernel/kernel_cpu.c
	$(C_C) $(FLAGS)		./kernel/kernel_cpu.c \
						-c \
						-o ./kernel/kernel_cpu.o

./kernel/kernel_vector.o:	./kernel/kernel_vector.h \
						./kernel/kernel_vector.c
	$(C_C) $(FLAGS) -DUSE_RISCV_VECTOR	./kernel/kernel_vector.c \
						-c \
						-o ./kernel/kernel_vector.o

./util/num/num.o:	./util/num/num.h \
					./util/num/num.c
	$(C_C) $(FLAGS) ./util/num/num.c \
					-c \
					-o ./util/num/num.o

./util/timer/timer.o:	./util/timer/timer.h \
						./util/timer/timer.c
	$(C_C) $(FLAGS)		./util/timer/timer.c \
						-c \
						-o ./util/timer/timer.o

# delete all object and executable files
clean:
	rm	*.o \
		./kernel/*.o \
		./util/num/*.o \
		./util/timer/*.o \
		./util/device/*.o; \
	rm bin/*.dump; \
	rm bin/*.exe;
