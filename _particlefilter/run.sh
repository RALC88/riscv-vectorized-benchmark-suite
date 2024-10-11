#!/bin/bash

echo " "
echo "----------------------------------------------------------------------------------"
echo "PARTICLEFILTER"
echo "----------------------------------------------------------------------------------"
echo " "

app="particlefilter"

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
echo "RUNNING PARTICLEFILTER"
echo "----------------------------------------------------------------------------------"

if [ $simsize == "tiny" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe -x 128 -y 128 -z 2 -np 256"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe -x 128 -y 128 -z 2 -np 256
	else #qemu
		echo "command: $RISCV/bin/qemu-riscv64 -L $RISCV/sysroot bin/${app}_${version}.exe -x 128 -y 128 -z 2 -np 256"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/qemu-riscv64 -L $RISCV/sysroot bin/${app}_${version}.exe -x 128 -y 128 -z 2 -np 256
	fi
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "small" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe -x 128 -y 128 -z 8 -np 1024"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe -x 128 -y 128 -z 8 -np 1024
	else #qemu
		echo "command: $RISCV/bin/qemu-riscv64 -L $RISCV/sysroot bin/${app}_${version}.exe -x 128 -y 128 -z 8 -np 1024"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/qemu-riscv64 -L $RISCV/sysroot bin/${app}_${version}.exe -x 128 -y 128 -z 8 -np 1024
	fi
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "medium" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe -x 128 -y 128 -z 16 -np 4096"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe -x 128 -y 128 -z 16 -np 4096
	else #qemu
		echo "command: $RISCV/bin/qemu-riscv64 -L $RISCV/sysroot bin/${app}_${version}.exe -x 128 -y 128 -z 16 -np 4096"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/qemu-riscv64 -L $RISCV/sysroot bin/${app}_${version}.exe -x 128 -y 128 -z 16 -np 4096
	fi
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "large" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe -x 128 -y 128 -z 24 -np 8192"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe -x 128 -y 128 -z 24 -np 8192
	else #qemu
		echo "command: $RISCV/bin/qemu-riscv64 -L $RISCV/sysroot bin/${app}_${version}.exe -x 128 -y 128 -z 24 -np 8192"
		echo "----------------------------------------------------------------------------------"
		$RISCV/bin/qemu-riscv64 -L $RISCV/sysroot bin/${app}_${version}.exe -x 128 -y 128 -z 24 -np 8192
	fi
	echo "----------------------------------------------------------------------------------"
else
	echo "RiVec Benchmark Suite"
fi
