#!/bin/bash
echo "Provision MR Script"
cp nameIPMapMR ~/mapred/Hadoop-ZMF/nameIPMapMR
cd ~/mapred/Hadoop-ZMF
./setupMR1.sh nameIPMapMR &
