#include "move.hpp"
#include <iostream>

Move::Move(float width, float height) : positionX_{0}, positionY_{0}, moveSpeed_{40}, minimumHeight_{-400}, maximumHeight_{height + 50}, minimumWidth_{-400}, maximumWidth_{width + 50} {}

void Move::goLeft(float zoom) {
    positionX_ -= moveSpeed_ * zoom;
    positionX_ = (positionX_ < minimumWidth_) ? minimumWidth_ : positionX_; // permet de limiter le déplacement, pour pas aller à l'infinie
}

void Move::goRight(float zoom) {
    positionX_ += moveSpeed_ * zoom; 
    positionX_ = (positionX_ > maximumWidth_) ? maximumWidth_ : positionX_; // permet de limiter le déplacement, pour pas aller à l'infinie
}

void Move::goTop(float zoom) {
    positionY_ -= moveSpeed_ * zoom;
    positionY_ = (positionY_ < minimumHeight_) ? minimumHeight_ : positionY_; // permet de limiter le déplacement, pour pas aller à l'infinie
}

void Move::goDown(float zoom) {
    positionY_ += moveSpeed_ * zoom;
    positionY_ = (positionY_ > maximumHeight_) ? maximumHeight_ : positionY_; // permet de limiter le déplacement, pour pas aller à l'infinie
}