#!/bin/bash

### SAVE CURRENT DIRECTORY ###
CURRENT_DIR=$(pwd)

echo "----------------------------------------------------------------------------------"
echo "RiVec Benchmark Suite"
echo "----------------------------------------------------------------------------------"
echo ""

while true; do
    read -p 'select one application to run [axpy blackscholes canneal jacobi-2d lavaMD matmul swaptions streamcluster somier particlefilter pathfinder]: ' app
    if [[ $app == "axpy" ]]  || [[ $app == "blackscholes" ]] || [[ $app == "canneal" ]] || [[ $app == "jacobi-2d" ]] || [[ $app == "lavaMD" ]] || [[ $app == "matmul" ]] || [[ $app == "swaptions" ]] || [[ $app == "streamcluster" ]] || [[ $app == "somier" ]] || [[ $app == "particlefilter" ]] || [[ $app == "pathfinder" ]]; then
        break
    else
    	echo "Input not valid, try again."
        continue
    fi
done

cd _$app
source run.sh

### RETURN ###
cd $CURRENT_DIR