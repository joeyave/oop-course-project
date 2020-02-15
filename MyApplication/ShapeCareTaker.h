#pragma once
#include "Shape.h"
#include <stack>
#include <fstream>

class ShapeCareTaker
{
public:
	ShapeCareTaker() {}
	ShapeCareTaker(sf::Shape* shp)
	{
		shape = shp;
	}

	void createSnapshot() // saves a new state of the particle
	{
		//memento_snapshot = shape->saveToMemento();

		json_snapshot = shape->saveToJson();
	}

	void undo()
	{
		//shape->restoreFromMemento(memento_snapshot);

		shape->restoreFromJson(json_snapshot);
	}

	void undo(Json::Value snap)
	{
		shape->restoreFromJson(snap);
	}
	
	Json::Value getJsonSnapshot() const
	{
		return json_snapshot;
	}

private:
	Json::Value json_snapshot;
	sf::Shape::Memento memento_snapshot;
	sf::Shape* shape;
};

