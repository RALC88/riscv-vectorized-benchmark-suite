#!/bin/bash

echo " "
echo "----------------------------------------------------------------------------------"
echo "BLACKSCHOLES"
echo "----------------------------------------------------------------------------------"
echo " "

app="blackscholes"

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
echo "RUNNING BLACKSCHOLES"
echo "----------------------------------------------------------------------------------"

if [ $simsize == "tiny" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 input/in_512.input output/output_prices_${version}_$simsize.txt"
		echo "----------------------------------------------------------------------------------"
		mkdir output
		echo " " > output/output_prices_${version}_$simsize.txt
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 input/in_512.input output/output_prices_${version}_$simsize.txt
	else
		echo "qemu: we need to mount the input file into qemu file system, looking on how to do it.. WIP"
	fi
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "small" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 input/in_4K.input output/output_prices_${version}_$simsize.txt"
		echo "----------------------------------------------------------------------------------"
		mkdir output
		echo " " > output/output_prices_${version}_$simsize.txt
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 input/in_4K.input output/output_prices_${version}_$simsize.txt
	else
		echo "qemu: we need to mount the input file into qemu file system, looking on how to do it.. WIP"
	fi
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "medium" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 input/in_16K.input output/output_prices_${version}_$simsize.txt"
		echo "----------------------------------------------------------------------------------"
		mkdir output
		echo " " > output/output_prices_${version}_$simsize.txt
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 input/in_16K.input output/output_prices_${version}_$simsize.txt
	else
		echo "qemu: we need to mount the input file into qemu file system, looking on how to do it.. WIP"
	fi
	echo "----------------------------------------------------------------------------------"
elif [ $simsize == "large" ]; then
	if [ $sim == "spike" ]; then
		echo "command: $RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 input/in_64K.input output/output_prices_${version}_$simsize.txt"
		echo "----------------------------------------------------------------------------------"
		mkdir output
		echo " " > output/output_prices_${version}_$simsize.txt
		$RISCV/bin/spike --isa=rv64gcv pk bin/${app}_${version}.exe 1 input/in_64K.input output/output_prices_${version}_$simsize.txt
	else
		echo "qemu: we need to mount the input file into qemu file system, looking on how to do it.. WIP"
	fi
	echo "----------------------------------------------------------------------------------"
else
	echo "RiVec Benchmark Suite"
fi
