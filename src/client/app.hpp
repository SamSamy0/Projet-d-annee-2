#pragma once
#include <SFML/Graphics.hpp>
#include "state.hpp"

using namespace std;

class App {
    State appState_;
    public :
        App();
        void run();
};