#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class Move {
	public :
		float positionX_;
		float positionY_;
		float moveSpeed_;
		float∏ minimumHeight_;
		float maximumHeight_;
		float minimumWidth_;
		float maximumWidth_;
		Move(float width, float height);
		void goLeft(float zoom);
		void goRight(float zoom);
		void goTop(float zoom);
		void goDown(float zoom);
};	
	

