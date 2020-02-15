#pragma once
#include "Shape.h"
#include "RenderWindow.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Composite : public sf::Shape
{
public:
	// De/Constructors.
	Composite()
	{
		type = "composite";
		update();
	}

	Composite (const Composite& obj)
	{
		type = "composite";

		for (auto& obj_shape : obj.shapes)
		{
			shapes.push_back(obj_shape->clone());
		}
	}

	// Setters.
	void setFillColor(const sf::Color& color) override
	{
		for (auto& shape : shapes)
			shape->setFillColor(color);
	}

	void setAlpha(const sf::Uint8 index) override
	{
		std::vector<sf::Color> colors;

		for (auto& shape : shapes)
			colors.push_back(shape->getFillColor());

		for (auto& color : colors)
			color.a = index;

		for (auto& shape : shapes)
		{
			shape->setFillColor(colors.back());
			colors.pop_back();
		}
	}

	void setScale(float factorX, float factorY) override
	{
		for (auto& shape : shapes)
		{
			shape->setScale(factorX, factorY);
		}
	}

	void setScale(const sf::Vector2f& factors) override
	{
		setScale(factors.x, factors.y);
	}


	void setOutlineThickness(float thickness) override
	{
		for (auto& shape : shapes)
		{
			shape->setOutlineThickness(thickness);
		}
	}

	// Getters.
	std::string getType() override
	{
		return type;
	}

	
	std::unordered_map<int, sf::Color> getFillColors()
	{
		std::unordered_map<int, sf::Color> fillColors;

		for (auto& shape : shapes)
		{
			fillColors[shape->getID()] = shape->getFillColor();
		}

		return fillColors;
	}

	void setFillColors(std::unordered_map<int, sf::Color> positions)
	{
		for (auto& shape : shapes)
		{
			shape->setFillColor(positions[shape->getID()]);
		}
	}

	const sf::Vector2f& getPosition() const override
	{
		return shapes[0]->getPosition();
	}

	std::unordered_map<int, sf::Vector2f> getPositions()
	{
		std::unordered_map<int, sf::Vector2f> positions;

		for (auto& shape : shapes)
		{
			positions[shape->getID()] = shape->getPosition();
		}

		return positions;
	}

	void setPositions(std::unordered_map<int, sf::Vector2f> positions)
	{
		for (auto& shape : shapes)
		{
			shape->setPosition(positions[shape->getID()]);
		}
	}

	std::unordered_map<int, sf::Vector2f> getScales()
	{
		std::unordered_map<int, sf::Vector2f> scales;

		for (auto& shape : shapes)
		{
			scales[shape->getID()] = shape->getScale();
		}

		return scales;
	}

	void setScales(std::unordered_map<int, sf::Vector2f> scales)
	{
		for (auto& shape : shapes)
		{
			shape->setScale(scales[shape->getID()]);
		}
	}

	std::vector<sf::Shape*>* getShapes()
	{
		return &shapes;
	}

	//Other methods.
	Memento saveToMemento() override
	{
		ShapeState newState;

		newState.shapeID = this->getID();

		newState.fillColors = this->getFillColors();
		newState.positions = this->getPositions();
		newState.scales = this->getScales();

		return Memento(newState);
	}

	void restoreFromMemento(Memento memento) override
	{
		ShapeState state = memento.getSavedState();

		setPositions(state.positions);
		setFillColors(state.fillColors);
		setScales(state.scales);
	}

	Shape* clone() override
	{
		return new Composite(*this);
	}

	void setToDefault() override
	{
		shapes.clear();
	}

	void addShape(sf::Shape* shp) override
	{
		shapes.push_back(shp);
		update();
	}

	void move(float offsetX, float offsetY) override
	{
		for (auto& shape : shapes)
			shape->move(offsetX, offsetY);
	}

	void move(const sf::Vector2f& offset) override
	{
		for (auto& shape : shapes)
			shape->move(offset);
	}

	void boundMoveActivate(sf::RenderWindow& wnd) override
	{
		for (auto& shape : shapes)
		{
			if (!dynamic_cast<Composite*>(shape) && shape)
			{
				if (shape->getPosition().x < 0)
				{
					shape->setPosition(sf::Vector2f(wnd.getSize().x, shape->getPosition().y));
				}
				if (shape->getPosition().x > wnd.getSize().x)
				{
					shape->setPosition(sf::Vector2f(0.0f, shape->getPosition().y));
				}
				if (shape->getPosition().y > wnd.getSize().y)
				{
					shape->setPosition(sf::Vector2f(shape->getPosition().x, 0.0f));
				}
				if (shape->getPosition().y < 0)
				{
					shape->setPosition(sf::Vector2f(shape->getPosition().x, wnd.getSize().y));
				}
			}
			else
			{
				shape->boundMoveActivate(wnd);
			}
		}
	}

	bool intersectsWith(sf::Shape* shp, sf::RenderWindow& wnd) override
	{
		for (auto& shape : shapes)
		{
			if (shape->intersectsWith(shp, wnd))
				return true;
		}
		return false;
	}

	sf::Uint8 containsTransparent()
	{
		for (auto& shape : shapes)
		{
			if (shape->getFillColor().a == 0)
				return true;
		}
		return false;
	}

	bool selectedWithMouse(sf::RenderWindow& wnd) override
	{
		for (auto& shape : shapes)
		{
			if (shape->selectedWithMouse(wnd))
				return true;
		}

		return false;
	}

	int getID() override
	{
		return shapes[0]->getID();
	}

	Json::Value saveToJson() override
	{
		Json::Value composite;
		Json::Value arr(Json::arrayValue);

		for (auto& shape : shapes)
		{
			Json::Value val;

			val["shape id"] = shape->getID();
			val["scale"]["x"] = shape->getScale().x;
			val["scale"]["y"] = shape->getScale().y;
			val["fill color"]["a"] = shape->getFillColor().a;
			val["fill color"]["b"] = shape->getFillColor().b;
			val["fill color"]["g"] = shape->getFillColor().g;
			val["fill color"]["r"] = shape->getFillColor().r;
			val["position"]["x"] = shape->getPosition().x;
			val["position"]["y"] = shape->getPosition().y;
			val["type"] = shape->getType();
			
			arr.append(val);
		}

		composite["composite shapes"] = arr;
		composite["shape id"] = this->getID();
		composite["type"] = this->getType();
		
		return composite;
	}

	void restoreFromJson(Json::Value composite_shape) override
	{
		composite_shape = composite_shape["composite shapes"];
		int i = 0;

		for (auto& shape : shapes)
		{
			sf::Color color(
				composite_shape[i]["fill color"]["r"].asUInt(),
				composite_shape[i]["fill color"]["g"].asUInt(),
				composite_shape[i]["fill color"]["b"].asUInt(),
				composite_shape[i]["fill color"]["a"].asUInt());

			sf::Vector2f position(
				composite_shape[i]["position"]["x"].asFloat(),
				composite_shape[i]["position"]["y"].asFloat());

			sf::Vector2f scale(
				composite_shape[i]["scale"]["x"].asFloat(),
				composite_shape[i]["scale"]["y"].asFloat());

			shape->setFillColor(color);
			shape->setPosition(position);
			shape->setScale(scale);

			i++;
		}
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for (auto& shape : shapes)
		{
			if (shape)
				target.draw(*shape);
		}
	}

	std::vector<sf::Shape*> shapes;
	std::string type;
};
