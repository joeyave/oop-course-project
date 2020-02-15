#pragma once
#include "Shape.h"

class Ellipse : public sf::Shape
{
public:
	// De/Constructors.
	Ellipse()
	{
		type = "ellipse";
		update();
	}

	Ellipse(const sf::Vector2f& rds)
	{
		type = "ellipse";
		radius = rds;
		update();
	}

	// Setters.
	void setToDefault() override
	{
		setRadius(sf::Vector2f(150.0f, 100.0f));
		setScale(1.0f, 1.0f);
		setFillColor(sf::Color::Yellow);
		setPosition(200, 1000);
		setOrigin(150.0f, 100.0f);
		update();
	}

	void setRadius(const sf::Vector2f& rds)
	{
		radius = rds;
		update();
	}

	// Getters.
	std::string getType() override
	{
		return type;
	}
	
	const sf::Vector2f& getRadius() const
	{
		return radius;
	}

	std::size_t getPointCount() const override
	{
		return 30;
	}

	sf::Vector2f getPoint(std::size_t index) const override
	{
		static const float pi = 3.141592654f;

		const float angle = index * 2 * pi / getPointCount() - pi / 2;
		const float x = std::cos(angle) * radius.x;
		const float y = std::sin(angle) * radius.y;

		return { radius.x + x, radius.y + y };
	}

	//Other methods.
	Shape* clone() override
	{
		auto ellipse = new Ellipse(*this);
		ellipse->incrementID();

		return ellipse;
	}

private:
	sf::Vector2f radius;
	std::string type;
};

