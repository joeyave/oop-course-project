#pragma once
#include <unordered_map>
#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "Composite.h"

class ShapesFactory
{
public:
	ShapesFactory()
	{
		prototypes_["circle"] = new Circle();
		prototypes_["rectangle"] = new Rectangle();
		prototypes_["ellipse"] = new Ellipse();
		prototypes_["composite"] = new Composite();
	}

	~ShapesFactory()
	{
		delete prototypes_["circle"];
		delete prototypes_["rectangle"];
		delete prototypes_["ellipse"];
		delete prototypes_["composite"];
	}

	sf::Shape* createPrototype(std::string type)
	{
		return prototypes_[type]->clone();
	}

private:
	std::unordered_map<std::string, sf::Shape*> prototypes_;
};

