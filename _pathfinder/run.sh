#!/bin/bash

echo " "
echo "----------------------------------------------------------------------------------"
echo "PATHFINDER"
echo "----------------------------------------------------------------------------------"
echo " "

app_name="pathfinder"

while true; do
    echo -n "do you want to run on spike or qemu [spike qemu gem5]: "
    read sim
    if [ $sim == "spike" ]; then
		simulator="$RISCV/bin/spike --isa=rv64gcv pk"
		break
	elif [ $sim == "qemu" ]; then
		//simulator="$RISCV/bin/qemu-riscv64 -L $RISCV/sysroot"
		echo "qemu is not working for pathfinder. Fix in progress. Try with a diferent option: "
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
		app_args="input/data_tiny.in"
		break
	elif [ $simsize == "small" ]; then
		app_args="input/data_small.in"
		break
	elif [ $simsize == "medium" ]; then
		app_args="input/data_medium.in"
		break
	elif [ $simsize == "large" ]; then
		app_args="input/data_large.in"
		break
    else
    	echo "Input not valid, try again."
        continue
    fi
done

echo "[RIVEC]---------------------------------------------------------------------- ----"
echo "[RIVEC]                          RUNNING PATHFINDER    							"
echo "[RIVEC]---------------------------------------------------------------------------"

echo "command: $simulator bin/${app_name}_${version}.exe $app_args"
echo "----------------------------------------------------------------------------------"
if [ $sim == "gem5" ]; then
	$simulator  --cmd="bin/${app_name}_${version}.exe" --options="$app_args"
else
	$simulator bin/${app_name}_${version}.exe $app_args
fi

echo "[RIVEC]---------------------------------------------------------------------- ----"
echo "[RIVEC]                          DONE 											"
echo "[RIVEC]---------------------------------------------------------------------------"