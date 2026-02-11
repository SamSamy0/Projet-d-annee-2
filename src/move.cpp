#include "move.hpp"

Move::Move() {}
void Move::goLeft() {positionX_ -= 200; }
void Move::goRight() {positionX_ += 200;}
void Move::goTop() {positionY_ += 200;}
void Move::goDown() {positionY_ -= 200;}