#!/bin/bash

echo "--------------Installation des dépendances------------------------"
sudo apt update
sudo apt upgrade
sudo apt install cmake libfreetype6-dev libx11-dev libxrandr-dev libxcursor-dev libudev-dev libgl1-mesa-dev libflac-dev libogg-dev libvorbis-dev libopenal-dev libxi-dev build-essential

rm -rf build
mkdir build
cd build

echo "---------------------Configuration------------------"
cmake ..


echo "---------------------------Compilation----------------------------"
make -j$(nproc)


if [ $? -eq 0 ]; then
    echo "--------------------------------------"
    echo "Compilation réussie !"
    echo "L'exécutable se trouve dans le dossier build : sfml-app"
    echo "--------------------------------------"
else
    echo "Erreur lors de la compilation."
    exit 1
fi
