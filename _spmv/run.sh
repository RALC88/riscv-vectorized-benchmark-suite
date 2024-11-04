#!/bin/bash

echo " "
echo "----------------------------------------------------------------------------------"
echo " SPMV 																		"
echo "----------------------------------------------------------------------------------"
echo " "

app_name="spmv"

while true; do
    echo -n "do you want to run on spike or qemu [spike qemu gem5]: "
    read sim
    if [ $sim == "spike" ]; then
		simulator="$RISCV/bin/spike --isa=rv64gcv pk"
		break
	elif [ $sim == "qemu" ]; then
		#simulator="$RISCV/bin/qemu-riscv64 -L $RISCV/sysroot"
		echo "qemu is not working for spmv. Try with a diferent option: "
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
		app_args="input/football.mtx input/football.verif"
		break
	elif [ $simsize == "small" ]; then
		app_args="input/lhr07.mtx input/lhr07.verif"
		break
	elif [ $simsize == "medium" ]; then
		app_args="input/venkat25.mtx input/venkat25.verif"
		break
	elif [ $simsize == "large" ]; then
		app_args="input/poisson3Db.mtx input/poisson3Db.verif"
		break
    else
    	echo "Input not valid, try again."
        continue
    fi
done

echo "[RIVEC]---------------------------------------------------------------------- ----"
echo "[RIVEC]                          RUNNING SPMV      								"
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