#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

using namespace std;

float Zoom::getZoom() const { return facteurZoom_; }

void Zoom::zoomIn() { 
            facteurZoom_ *= 0.9f;
            if (facteurZoom_ < minZoom_) { // check le fait de ne pas dépasser la borne inférieur
                facteurZoom_ = minZoom_;
            }
        }
void Zoom::zoomOut() {
            facteurZoom_ *= 1.1f;
            if (facteurZoom_ > maxZoom_) { // check le fait de ne pas dépasser la borne supérieur
                facteurZoom_ = maxZoom_;
            }
        }