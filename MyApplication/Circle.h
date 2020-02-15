#pragma once
//#include <SFML/Graphics.hpp>
#include "Shape.h"

class Circle : public sf::Shape
{
public:
	// De/Constructors.
	Circle(const std::size_t pts = 30)
	{
		points = pts;
		type = "circle";
		update();
	}

	Circle(const float rds, const std::size_t pts = 30)
	{
		radius = rds;
		points = pts;
		type = "circle";
		update();
	}

	// Setters.
	void setToDefault()
	{
		setRadius(200.0f);
		setScale(1.0f, 1.0f);
		setFillColor(sf::Color::Magenta);
		setPosition(1700, 760);
		setOrigin(200, 200);
		update();
	}

	void setRadius(const float& rds)
	{
		radius = rds;
		update();
	}

	// Getters.
	std::string getType() override
	{
		return type;
	}
	
	const float& getRadius() const
	{
		return radius;
	}

	std::size_t getPointCount() const override
	{
		return points;
	}

	sf::Vector2f getPoint(std::size_t index) const override
	{
		static const float pi = 3.141592654f;

		float angle = index * 2 * pi / getPointCount() - pi / 2;
		float x = std::cos(angle) * radius;
		float y = std::sin(angle) * radius;

		return { radius + x, radius + y };
	}

	// Other functions.
	Shape* clone() override
	{
		auto circle = new Circle(*this);
		circle->incrementID();

		return circle;
	}

private:
	float radius;
	std::size_t points;
	std::string type;
};

