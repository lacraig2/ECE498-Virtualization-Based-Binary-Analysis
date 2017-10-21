#!/bin/bash


cd ECE498-Virtualization-Based-Binary-Analysis
echo "Updating git..."
git pull origin master
echo "Copying data..."
cp -r branch_tracker $PANDA_PLUGIN
cd ../build-panda
echo "Building..."
sudo $PANDA/build.sh
cd ..
