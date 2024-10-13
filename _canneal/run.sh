#!/bin/bash

echo " "
echo "----------------------------------------------------------------------------------"
echo "CANNEAL"
echo "----------------------------------------------------------------------------------"
echo " "

app="canneal"

while true; do
    echo -n "do you want to run on spike or qemu [spike qemu]: "
    read sim
    if [[ $sim == "spike" ]]  || [[ $sim == "qemu" ]]; then
        break
    else
    	echo "Input not valid, try again."
        continue
    fi
done


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
echo "RUNNING CANNEAL"
echo "----------------------------------------------------------------------------------"

if [ $simsize == "tiny" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 100 300 input/100.nets 8"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 4 300 input/10.nets 1
	else
		echo "qemu: we need to mount the input file into qemu file system, looking on how to do it.. WIP"
	fi
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "small" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 10000 2000 input/100000.nets 32"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 10000 2000 input/100000.nets 32
	else
		echo "qemu: we need to mount the input file into qemu file system, looking on how to do it.. WIP"
	fi
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "medium" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 15000 2000 input/200000.nets 64"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 15000 2000 input/200000.nets 64
	else
		echo "qemu: we need to mount the input file into qemu file system, looking on how to do it.. WIP"
	fi
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "large" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 15000 2000 input/400000.nets 128"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 15000 2000 input/400000.nets 128
	else
		echo "qemu: we need to mount the input file into qemu file system, looking on how to do it.. WIP"
	fi
	echo "----------------------------------------------------------------------------------"
else
	echo "RiVec Benchmark Suite"
fi
