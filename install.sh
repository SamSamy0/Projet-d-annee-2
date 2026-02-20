#!/bin/bash

echo "Installation des dépendances : "
sudo apt update
sudo apt install -y build-essential cmake sfml tgui

if [ ! -d "build" ]; then
    echo "--- Création du dossier build ---"
    mkdir build
fi


cd build

echo "Configuration : "
cmake ..


echo "Compilation : "
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

