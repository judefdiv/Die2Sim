#!/bin/bash

echo "Simulating RSFQ circuit"

defFile=$1
cirFile=${defFile%.def}.cir
csvFile=${defFile%.def}.csv

echo "Executing: Die2Sim -j $defFile -o $cirFile"
./build/Die2Sim -j $defFile -o $cirFile && \
echo "--------------------------------------------------"

echo "Executing: JoSIM $cirFile -o $csvFile"
josim $cirFile -o $csvFile && \
echo "--------------------------------------------------"

echo "Executing: pyDis $csvFile"
python3 pyDis.py $csvFile
echo "--------------------------------------------------"

echo "Done."