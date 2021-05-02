#!/bin/bash

echo -e "I)--------------------------------------------RUNNING ONE-ONE TESTING-------------------------------------------------"
echo ""
echo ""
cd one-one
./test.sh
echo ""
echo ""
cd ..
echo ""
echo ""
echo -e "II)-------------------------------------------RUNNING MANY-ONE TESTING-------------------------------------------------"
echo ""
echo ""
cd many-one
./test.sh
echo ""
echo ""