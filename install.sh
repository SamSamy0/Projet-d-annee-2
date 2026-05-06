#!/bin/bash

echo "--------------Installation des dépendances------------------------"
sudo apt update
sudo apt upgrade
sudo apt install cmake libfreetype6-dev libx11-dev libxrandr-dev libxcursor-dev libudev-dev libgl1-mesa-dev libflac-dev libogg-dev libvorbis-dev libopenal-dev libxi-dev build-essential qt6-base-dev qt6-tools-dev

cd build
shopt -s extglob
rm -rf !(CMakeLists.txt)

echo "---------------------Configuration------------------"
cmake .

echo "---------------------------Compilation----------------------------"
make -j$(nproc)


if [ $? -eq 0 ]; then
    echo "--------------------------------------"
    echo "Compilation réussie !"
    echo "Les executables se trouvent dans le dossier bin"
    echo "--------------------------------------"
else
    echo "Erreur lors de la compilation."
    exit 1
fi
