#include "zoom.hpp"


Zoom::Zoom() : facteurZoom_{1} {}

float Zoom::getZoom() const { return facteurZoom_; }

void Zoom::zoomIn() { 
            facteurZoom_ *= 0.9;
            if (facteurZoom_ < 0.01) { // met une limite sur le zoom avant
                facteurZoom_ = 0.01;
            }
        }
void Zoom::zoomOut() {
            facteurZoom_ *= 1.1;
            if (facteurZoom_ > 3.5) { // met une limite sur le zoom arrière
                facteurZoom_ = 3.5;
            }
        }
