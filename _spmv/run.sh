#!/bin/bash

echo " "
echo "----------------------------------------------------------------------------------"
echo "AXPY"
echo "----------------------------------------------------------------------------------"
echo " "


while true; do
    echo -n "do you want to run the serial or vectorized version [serial vector]: "
    read version
    if [[ $version == "serial" ]]  || [[ $version == "vector" ]]; then
        break
    else
    	echo "Input not valid, try again."
        continue
    fi
done

while true; do
    echo -n "select the simulation size [tiny small medium large]: "
    read simsize
    if [[ $simsize == "tiny" ]]  || [[ $simsize == "small" ]] || [[ $simsize == "medium" ]] || [[ $simsize == "large" ]]; then
        break
    else
    	echo "Input not valid, try again."
        continue
    fi
done

echo " "
echo "----------------------------------------------------------------------------------"
echo "RUNNING AXPY"
echo "----------------------------------------------------------------------------------"

if [ $simsize == "tiny" ]; then
	echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/axpy_$version.exe "256""
	echo "----------------------------------------------------------------------------------"
	$RISCV/bin/spike --isa=rv64gcv pk bin/axpy_$version.exe "256"
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "small" ]; then
	echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/axpy_$version.exe "512""
	echo "----------------------------------------------------------------------------------"
	$RISCV/bin/spike --isa=rv64gcv pk bin/axpy_$version.exe "512"
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "medium" ]; then
	echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/axpy_$version.exe "1024""
	echo "----------------------------------------------------------------------------------"
	$RISCV/bin/spike --isa=rv64gcv pk bin/axpy_$version.exe "1024"
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "large" ]; then
	echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/axpy_$version.exe "2048""
	echo "----------------------------------------------------------------------------------"
	$RISCV/bin/spike --isa=rv64gcv pk bin/axpy_$version.exe "2048"
	echo "----------------------------------------------------------------------------------"
else
	echo "RiVec Benchmark Suite"
fi