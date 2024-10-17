#!/bin/bash

echo " "
echo "----------------------------------------------------------------------------------"
echo "BLACKSCHOLES 																		"
echo "----------------------------------------------------------------------------------"
echo " "

app_name="blackscholes"

while true; do
    echo -n "do you want to run on spike or qemu [spike qemu gem5]: "
    read sim
    if [ $sim == "spike" ]; then
		simulator="$RISCV/bin/spike --isa=rv64gcv pk"
		break
	elif [ $sim == "qemu" ]; then
		#simulator="$RISCV/bin/qemu-riscv64 -L $RISCV/sysroot"
		echo "qemu is not working for blackscholes. Try with a diferent option: "
        continue
	elif [ $sim == "gem5" ]; then
		simulator="$GEM5/build/RISCV/gem5.opt $GEM5/configs/deprecated/example/se.py"
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
    if [ $simsize == "tiny" ]; then
		app_args="1 input/in_512.input output/output_prices_${version}_${simsize}.txt"
		break
	elif [ $simsize == "small" ]; then
		app_args="1 input/in_4K.input output/output_prices_${version}_${simsize}.txt"
		break
	elif [ $simsize == "medium" ]; then
		app_args="1 input/in_16K.input output/output_prices_${version}_${simsize}.txt"
		break
	elif [ $simsize == "large" ]; then
		app_args="1 input/in_64K.input output/output_prices_${version}_${simsize}.txt"
		break
    else
    	echo "Input not valid, try again."
        continue
    fi
done

echo "[RIVEC]---------------------------------------------------------------------- ----"
echo "[RIVEC]                          RUNNING BLACKSCHOLES								"
echo "[RIVEC]---------------------------------------------------------------------------"

echo "command: $simulator bin/${app_name}_${version}.exe $app_args"
echo "----------------------------------------------------------------------------------"
mkdir output
echo " " > output/output_prices_${version}_${simsize}.txt
if [ $sim == "gem5" ]; then
	$simulator  --cmd="bin/${app_name}_${version}.exe" --options="$app_args"
else
	$simulator bin/${app_name}_${version}.exe $app_args
fi

echo "[RIVEC]---------------------------------------------------------------------- ----"
echo "[RIVEC]                          DONE 											"
echo "[RIVEC]---------------------------------------------------------------------------"