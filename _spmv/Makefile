#makefile

# Variables for the compiler and options
RISCV_BIN=${RISCV}/bin
PK=${RISCV}/riscv64-unknown-elf/bin/pk

# Variables for source code
SRC=src/spmv.c src/main.c
BIN_DIR=bin
BIN_SERIAL=$(BIN_DIR)/spmv_serial.exe
BIN_VECTOR=$(BIN_DIR)/spmv_vector.exe
DUMP_SERIAL=$(BIN_DIR)/spmv_serial.dump
DUMP_VECTOR=$(BIN_DIR)/spmv_vector.dump

start:
	rm -f bin/*.*

serial:
	@echo "Compiling serial version..."
	$(RISCV_BIN)/clang -march=rv64gc -O2 -o $(BIN_SERIAL) $(SRC)
	$(RISCV_BIN)/llvm-objdump --mattr=+m,+f,+d,+a,+c -ds $(BIN_SERIAL) > $(DUMP_SERIAL)

vector:
	@echo "Compiling vector version..."
	$(RISCV_BIN)/clang -march=rv64gcv -fno-vectorize -DUSE_RISCV_VECTOR -O2 -I../common -o $(BIN_VECTOR) $(SRC)
	$(RISCV_BIN)/llvm-objdump --mattr=+m,+f,+d,+a,+c,+v -S $(BIN_VECTOR) > $(DUMP_VECTOR)

clean:
	rm -f bin/*.dump; \
	rm -f bin/*.exe;
