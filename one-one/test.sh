#!/bin/bash
echo ""
echo ""
echo -e "I)--------------------------------------------RUNNING ONE-ONE TESTING-------------------------------------------------"
echo ""
echo ""
echo -e "1)----------------------------------RUNNING APITEST TEST-----------------------------"
echo "./exe/apitest"
./exe/apitest
echo ""
echo ""
echo -e "2)----------------------------------RUNNING SPINLOCK TEST----------------------------"
echo "./exe/spinlocktest"
echo "Wait for a minute"
./exe/spinlocktest
echo ""
echo ""
echo -e "3)-----------------------------------RUNNING MUTEX TEST-------------------------------"
echo "./exe/mutextest"
echo "Wait for a minute"
./exe/mutextest
echo ""
echo ""
echo -e "4)-----------------------------------RUNNING MATRIX MULTIPLICATION TEST-----------------------------"
echo "./exe/matrix"
echo "Reads matrix from matrix.txt"
./exe/matrix
echo ""
echo ""
