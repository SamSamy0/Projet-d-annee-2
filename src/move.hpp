#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include <view.hpp>

using namespace std;

class Move {
	View& view_;
	public :
		Move(const View& view) : view_{view} {}
		void goLeft() {}
};	
	

