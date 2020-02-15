#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <unordered_map>

class ShapeState
{
public:
	int shapeID;
	
	sf::Vector2f scale;
	sf::Color fillColor;
	sf::Vector2f position;

	std::unordered_map<int, sf::Vector2f> positions;
	std::unordered_map<int, sf::Color> fillColors;
	std::unordered_map<int, sf::Vector2f> scales;
};

