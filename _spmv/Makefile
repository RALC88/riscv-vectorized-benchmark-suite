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

# Targets for building and running
all: help

help:
	@echo "Available commands:"
	@echo "  make help   - Display this information "
	@echo "  make build  - Compile RISCV binary "
	@echo "  make run    - Run the simulation "
	@echo "  INPUT       - Input file "
	@echo "  TYPE        - Select a version [serial|vector]. Default: serial "
	@echo "  SIMULATOR   - Select a simulator [spike|qemu]. Default: spike "
	@echo "  - - - "
	@echo "  make build TYPE=[serial|vector] "
	@echo "  make run SIMULATOR=[spike|qemu] TYPE=[serial|vector] INPUT=input/Na5.mtx"

# Compilation
build: 
ifeq ($(TYPE),vector)
	@echo "Compiling vector version..."
	$(RISCV_BIN)/clang -march=rv64gcv -DUSE_RISCV_VECTOR -O2 -I../common -o $(BIN_VECTOR) $(SRC)
	$(RISCV_BIN)/llvm-objdump --mattr=+m,+f,+d,+a,+c,+v -S $(BIN_VECTOR) > $(DUMP_VECTOR)
else
	@echo "Compiling serial version..."
	$(RISCV_BIN)/clang -march=rv64gc -O2 -o $(BIN_SERIAL) $(SRC)
	$(RISCV_BIN)/llvm-objdump --mattr=+m,+f,+d,+a,+c -ds $(BIN_SERIAL) > $(DUMP_SERIAL)
endif

# Simulation
run:
ifeq ($(SIMULATOR),qemu)
ifeq ($(TYPE),vector)
	@echo "Running with QEMU the vector version..."
	qemu-riscv64 $(BIN_VECTOR) $(INPUT)
else
	@echo "Running with QEMU the serial version..."
	qemu-riscv64 $(BIN_SERIAL) $(INPUT)
endif
else
ifeq ($(TYPE),vector)
	@echo "Running with Spike the vector version..."
	spike --isa=rv64gcv $(PK) $(BIN_VECTOR) $(INPUT)
else
	@echo "Running with Spike the serial version..."
	spike --isa=rv64gc $(PK) $(BIN_SERIAL) $(INPUT)
endif
endif

# Clean up
clean:
	rm -f $(BIN_DIR)/*.exe $(BIN_DIR)/*.dump
