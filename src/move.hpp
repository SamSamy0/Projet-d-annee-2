#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class Move {
	public :
		float positionX_{600};
		float positionY_{400};
		Move();
		void goLeft();
		void goRight();
		void goTop();
		void goDown();
};	
	

