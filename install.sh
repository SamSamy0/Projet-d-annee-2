#!/bin/bash

echo "--------------Installation des dépendances------------------------"
sudo apt update
sudo apt upgrade
sudo apt install -y build-essential cmake

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
    echo "L'exécutable se trouve à la racine : ../sfml-app"
    echo "--------------------------------------"
else
    echo "Erreur lors de la compilation."
    exit 1
fi

