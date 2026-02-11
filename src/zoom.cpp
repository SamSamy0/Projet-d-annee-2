#include "zoom.hpp"

using namespace std;

float Zoom::getZoom() const { return facteurZoom_; }

void Zoom::zoomIn() { 
            facteurZoom_ *= 0.9f;
            if (facteurZoom_ < 0.25) { // check le fait de ne pas dépasser la borne inférieur
                facteurZoom_ = 0.25;
            }
        }
void Zoom::zoomOut() {
            facteurZoom_ *= 1.1f;
            if (facteurZoom_ > 4) { // check le fait de ne pas dépasser la borne supérieur
                facteurZoom_ = 4;
            }
        }