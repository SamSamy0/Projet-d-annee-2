#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class Zoom {
    float facteurZoom_ = 1;
    const float minZoom_ = 0.25;
    const float maxZoom_ = 4.0;
    public :
        float getZoom() const;
        void zoomIn() { // zoom avant sur la carte
            facteurZoom_ *= 0.9f;
            if (facteurZoom_ < minZoom_) { // check le fait de ne pas dépasser la borne inférieur
                facteurZoom_ = minZoom_;
            }
        }
        void zoomOut() { // zoom arrière sur la carte
            facteurZoom_ *= 1.1f;
            if (facteurZoom_ > maxZoom_) { // check le fait de ne pas dépasser la borne supérieur
                facteurZoom_ = maxZoom_;
            }
        }
};
