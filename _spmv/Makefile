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

ERROR_TYPE := "Error: A version needs to be specified using TYPE=version."
ERROR_FILE := "Error: A input file needs to be specified using INPUT=file.mtx."
ERROR_SIMULATOR := "Error: A simulator needs to be specified using SIMULATOR=version."
ERROR_UNSUPPORTED := "Error: Unsupported option: "

# Targets for building and running
all: help

help:
	@echo "   "
	@echo "  make build TYPE=[serial|vector] "
	@echo "  make run SIMULATOR=[spike|qemu] TYPE=[serial|vector] INPUT=input/Na5.mtx"
	@echo "   "
	@echo "Commands:"
	@echo "  make help   - Display this information. "
	@echo "  make build  - Compile RISCV binary. "
	@echo "  make run    - Run the simulation. "
	@echo "  make clean  - Delete the artifacts in the bin folder. "
	@echo "Options:"
	@echo "  INPUT       - Input file (*.mtx)"
	@echo "  TYPE        - Select a version [serial|vector]. "
	@echo "  SIMULATOR   - Select a simulator [spike|qemu]. "
	@echo "  "

# Compilation
build:
ifeq ($(TYPE),) 
	@echo $(ERROR_TYPE)
endif
ifeq ($(TYPE),vector)
	@echo "Compiling vector version..."
	$(RISCV_BIN)/clang -march=rv64gcv -fno-vectorize -DUSE_RISCV_VECTOR -O2 -I../common -o $(BIN_VECTOR) $(SRC)
	$(RISCV_BIN)/llvm-objdump --mattr=+m,+f,+d,+a,+c,+v -S $(BIN_VECTOR) > $(DUMP_VECTOR)
else ifeq ($(TYPE),serial)
	@echo "Compiling serial version..."
	$(RISCV_BIN)/clang -march=rv64gc -O2 -o $(BIN_SERIAL) $(SRC)
	$(RISCV_BIN)/llvm-objdump --mattr=+m,+f,+d,+a,+c -ds $(BIN_SERIAL) > $(DUMP_SERIAL)
else
	@echo $(ERROR_UNSUPPORTED)$(TYPE)
endif

# Simulation
run:
ifeq ($(TYPE),) 
	$(error $(ERROR_TYPE))
endif
ifeq ($(SIMULATOR),) 
	$(error $(ERROR_SIMULATOR))
endif
ifeq ($(INPUT),) 
	$(error $(ERROR_FILE))
endif
ifeq ($(SIMULATOR),qemu)
ifeq ($(TYPE),vector)
	@echo "Running with QEMU the vector version..."
	qemu-riscv64 $(BIN_VECTOR) $(INPUT)
else ifeq ($(TYPE),serial)
	@echo "Running with QEMU the serial version..."
	qemu-riscv64 $(BIN_SERIAL) $(INPUT)
else
	@echo $(ERROR_UNSUPPORTED)$(TYPE)
endif
else ifeq ($(SIMULATOR),spike)
ifeq ($(TYPE),vector)
	@echo "Running with Spike the vector version..."
	spike --isa=rv64gcv $(PK) $(BIN_VECTOR) $(INPUT)
else ifeq ($(TYPE),serial)
	@echo "Running with Spike the serial version..."
	spike --isa=rv64gc $(PK) $(BIN_SERIAL) $(INPUT)
else
	@echo $(ERROR_UNSUPPORTED)$(TYPE)
endif
else
	@echo $(ERROR_UNSUPPORTED)$(SIMULATOR)
endif

# Clean up
clean:
	rm -f $(BIN_DIR)/*.exe $(BIN_DIR)/*.dump
